
#include "GenerationClassForMetricold.h"

using namespace SparCraft;

GenerationClassForMetricold::GenerationClassForMetricold(const IDType& playerID) {
    _playerID = playerID;
    iniciarAlphaBeta();
    pgs = new PortfolioGreedySearchNoTime(_playerID, PlayerModels::NOKDPS, 1, 0, 40);
    lastTime = 0;
    _controlNumAbs = 0;
    std::cout << "Instancia da Classe  GenerationClassForMetric " << std::endl;
}

void GenerationClassForMetricold::getMoves(GameState& state, const MoveArray& moves, std::vector<Action>& moveVec) {
    //controla a inicialização das abstrações
    if (_vecAbstracao.size() == 0) {
        iniciaAbstracao(state);
    }

    Timer t;
    t.start();
    moveVec.clear();
    UnitScriptData currentScriptData;
    double ms;
    //state.print();
    if (lastTime > state.getTime()) {
        _unitAbsAB.clear();
    }
    lastTime = state.getTime();
    
    if(_unitAbsAB.size() == 0){
        for(auto & un : _vecAbstracao[_controlNumAbs]){
            _unitAbsAB.insert(state.getUnitByID(_playerID, un.ID()));
        }
        
        
        std::cout<<"Abstração setada = "<< _controlNumAbs << " com as unidades:"<<std::endl;
        for(auto & un : _unitAbsAB){
            un.print();
        }
        
        
        
        _controlNumAbs++;
    }
    

    //estado que será utilizado para simular as variações necessárias do AB
    GameState newState;
    //vetores ordenados por distancia que conterão as unidades
    std::vector<Unit> unidadesAliadas;
    std::vector<Unit> unidadesInimigas;
    moveVec.clear();
    _unAttack.clear();

    //obtem nossa unidade inicial.
    const Unit & ourUnit(state.getUnit(_playerID, (0)));

    listaOrdenadaForMoves(_playerID, ourUnit, state, unidadesAliadas, moves);

    //incluído para validação da inicializacao
    listaOrdenada(state.getEnemy(_playerID), ourUnit, state, unidadesInimigas);

    //início validação por movimentação
    if (applyClosestInicialization(unidadesAliadas, unidadesInimigas, state)) {
        //criar abstração com todas as unidades aliadas contra a centróide do exército inimigo
        // for este tipo de inicialização indico o alpha-beta com 40 ms
        alphaBeta->setLimitTime(40);
        //adiciono todas as unidades aliadas
        for (auto & un : unidadesAliadas) {
            newState.addUnitWithID(un);
        }
        //Seleciono a unidade com base na média das distancias euclidianas.
        newState.addUnitWithID(getCalculateEnemy(newState, unidadesInimigas));

        //aplico AB
        doAlphaBeta(newState, moveVec, state);

        //limpo o state por segurança        
        copiarStateCleanUnit(state, newState);

    } else {


        //obtenho os movimentos sugeridos pelo PGS
        ms = t.getElapsedTimeInMilliSec();
        StateEvalScore bestScore;
        currentScriptData = pgs->searchForScripts(_playerID, state, bestScore);
        ms = t.getElapsedTimeInMilliSec() - ms;
        //std::cout << " Tempo total do PGS "<< ms << std::endl;

        controlUnitsForAB(state, moves);

        std::vector<Action> moveVecPgs, movecAB;

        MoveArray movesPGS;
        state.generateMoves(movesPGS, _playerID);
        GameState copy(state);
        currentScriptData.calculateMoves(_playerID, movesPGS, copy, moveVecPgs);

        if (unitsInMoves(state, moves) and ((40 - ms) > 4)) {
            //Executo o AB
            std::set<IDType> unitAbsAB;
            for (auto & un : _unitAbsAB) {
                unitAbsAB.insert(un.ID());
            }
            //std::cout << " Tempo total para AB "<< 40 - ms << std::endl;
            alphaBeta->setLimitTime(40 - ms);
            StateEvalScore bestScore;
            alphaBeta->doSearchWithMoves(state, currentScriptData, unitAbsAB, _playerID, bestScore);
            movecAB.assign(alphaBeta->getResults().bestMoves.begin(), alphaBeta->getResults().bestMoves.end());

            //tentativa de utilizar o playout para julgar qual seria a melhor execução Ab-PGS ou PGS.
            UnitScriptData baseScriptData;
            const IDType enemyPlayer(state.getEnemy(_playerID));
            //inicializar o baseScriptData com NO-KDPS
            //Player
            for (size_t unitIndex(0); unitIndex < state.numUnits(_playerID); ++unitIndex) {
                baseScriptData.setUnitScript(state.getUnit(_playerID, unitIndex), SparCraft::PlayerModels::NOKDPS);
            }
            //Enemy
            for (size_t unitIndex(0); unitIndex < state.numUnits(enemyPlayer); ++unitIndex) {
                baseScriptData.setUnitScript(state.getUnit(enemyPlayer, unitIndex), SparCraft::PlayerModels::NOKDPS);
            }

            std::vector<Action> moveVecPgsEnemy;
            if (state.bothCanMove()) {
                //gero os movimentos inimigos
                MoveArray movesPGSEnemy;
                state.generateMoves(movesPGSEnemy, enemyPlayer);
                GameState copy2(state);
                baseScriptData.calculateMoves(enemyPlayer, movesPGSEnemy, copy2, moveVecPgsEnemy);
            }

            //Execução AB-PGS
            Game gABPGS(state, 50);
            gABPGS.getState().makeMoves(movecAB);
            if (gABPGS.getState().bothCanMove()) {
                gABPGS.getState().makeMoves(moveVecPgsEnemy);
            }
            gABPGS.getState().finishedMoving();
            gABPGS.playIndividualScripts(baseScriptData);

            //Execução PGS
            Game gPGS(state, 50);
            gPGS.getState().makeMoves(moveVecPgs);
            if (gPGS.getState().bothCanMove()) {
                gPGS.getState().makeMoves(moveVecPgsEnemy);
            }
            gPGS.getState().finishedMoving();
            gPGS.playIndividualScripts(baseScriptData);

            if (gABPGS.getState().eval(_playerID, SparCraft::EvaluationMethods::LTD2) >
                    gPGS.getState().eval(_playerID, SparCraft::EvaluationMethods::LTD2)) {
                moveVec.assign(alphaBeta->getResults().bestMoves.begin(), alphaBeta->getResults().bestMoves.end());
                //std::cout << "Escolhemos o ABPGS" << std::endl;
            } else {
                moveVec = moveVecPgs;
            }

        } else {
            moveVec = moveVecPgs;
        }

    }
    //Lista as informações básicas para as Métricas
    if(_unitAbsAB.size() > 0){
        this->printMedia(state);
    }


    /*
    std::cout << "************* INICIO GenerationClass  **************" << std::endl;
    for (auto & ac : moveVec) {
        std::cout << ac.debugString() << std::endl;
    }
    std::cout << "************* FIM GenerationClass  **************" << std::endl;
    std::cout << "##################################################" << std::endl;
     */
    ms = t.getElapsedTimeInMilliSec();
    //printf("\nGenerationClass   Execução completa %lf ms\n", ms);


    /*
    MoveArray movesPGS;
    state.generateMoves(movesPGS, _playerID);
    std::vector<Action> moveVecPgs;
    GameState copy(state);
    currentScriptData.calculateMoves(_playerID, movesPGS, copy, moveVecPgs);
    std::cout<<"************* INICIO GenerationClass PGS **************"<<std::endl;
    for(auto & ac : moveVecPgs){
        std::cout<<ac.debugString()<<std::endl;
    }
    std::cout<<"************* FIM GenerationClass PGS **************"<<std::endl;
    std::cout<<"##################################################"<<std::endl;
     */

}

bool GenerationClassForMetricold::unitsInMoves(GameState& state, const MoveArray& moves) {
    for (size_t unitIndex(0); unitIndex < moves.numUnits(); ++unitIndex) {
        const Unit & unit = state.getUnit(_playerID, unitIndex);
        for (auto & un : _unitAbsAB) {
            if (un.ID() == unit.ID()) {
                return true;
            }
        }
    }

    return false;
}

//separo as unidades que serão utilizadas para compor a abstração que será utilizada no AB
//e faço controle e manutenção destas

void GenerationClassForMetricold::controlUnitsForAB(GameState & state, const MoveArray & moves) {
    int numUnits = 4;
    //verifico se as unidades não foram mortas
    std::set<Unit, lex_cm> tempUnitAbsAB;
    for (auto & un : _unitAbsAB) {
        if (state.unitExist(_playerID, un.ID())) {
            tempUnitAbsAB.insert(un);
        }
    }
    _unitAbsAB = tempUnitAbsAB;

    if (state.numUnits(_playerID) <= numUnits) {
        _unitAbsAB.clear();
        //adiciono todas as unidades para serem controladas pelo AB
        for (int u(0); u < state.numUnits(_playerID); ++u) {
            _unitAbsAB.insert(state.getUnit(_playerID, u));
        }
    } else if (!(_unitAbsAB.size() == numUnits)) {

        if ((state.numUnits(_playerID) < 2 or moves.numUnits() < 2)
                and _unitAbsAB.size() == 0) {
            _unitAbsAB.insert(state.getUnit(_playerID, 0));
        } else {
            int control = 0;
            while (_unitAbsAB.size() < numUnits and control < 20) {
                _unitAbsAB.insert(state.getUnit(_playerID, rand() % state.numUnits(_playerID)));
                control++;
            }
        }
    }

}

void GenerationClassForMetricold::analisarAbstractForm(GameState newState, std::vector<Unit> unidadesInimigas) {
    //obtenho a unidade inimiga contida na abstração
    Unit & enemy = newState.getUnit(newState.getEnemy(_playerID), 0);

    //obtenho as unidades aliadas contidas no estado
    std::vector<Unit> unAlly;


    for (auto & un : unAlly) {

    }


}

/*
 *  Função que consiste do processo de analisar se exitem outras unidades inimigas que podem
 * ser atacadas pelas unidades contidas na abstração e adicionar estas unidades inimigas ao estado.
 */
void GenerationClassForMetricold::addMoreEnemy(GameState& newState, std::vector<Unit>& unInimigas) {
    if (unInimigas.size() > 0) {
        //obter unidades aliadas da abstração
        std::vector<Unit> unAl;
        for (int u(0); u < newState.numUnits(_playerID); ++u) {
            unAl.push_back(newState.getUnit(_playerID, u));
        }

        //verificar se existe no vetor alguma outra unidade inimiga que pode ser inserida.
        for (auto & unIn : unInimigas) {
            if (!newState.unitExist(newState.getEnemy(_playerID), unIn.ID())) {
                //verifico se as unidades amigas podem atacar
                for (auto & uAmiga : unAl) {
                    if (uAmiga.canAttackTarget(unIn, newState.getTime())) {
                        if (!newState.unitExist(newState.getEnemy(_playerID), unIn.ID())) {
                            newState.addUnitWithID(unIn);
                        }
                    }
                }
            }
        }
    }
}

/*Função que analisa as unidades aliadas e as unidades inimigas indicando se existe alguma unidade aliada capaz de 
 * atacar alguma unidade aliada.
 * Para retornar true tem que atender aos seguintes critérios:
 *  -> O numero de unidades aliadas que tem movimentos tem que ser o mesmo numero de unidades existentes no estado
 *  -> Nenhuma unidade aliada pode ser capaz de atacar alguma unidade inimiga
 *  & unAliadas     - Vetor com todas as unidades que possuem movimentos
 *  & unInimigas    - Vetor com todas as unidades inimigas
 *  & state         - Estado real do jogo
 */
bool GenerationClassForMetricold::applyClosestInicialization(std::vector<Unit> & unAliadas, std::vector<Unit> & unInimigas, GameState & state) {
    if (unAliadas.size() != state.numUnits(_playerID)) {
        return false;
    }

    for (auto & unAl : unAliadas) {
        for (auto & unEn : unInimigas) {
            if (unAl.canAttackTarget(unEn, state.getTime())) {
                return false;
            }
        }
    }

    return true;
}

//função que analisa os movimentos sugeridos pelo AB e busca encontrar ataques perdidos
//funciona apenas com 1 unidade inimiga

void GenerationClassForMetricold::removeLoseAttacks(GameState& newState, std::vector<Action>& moveVec, GameState & state) {
    _UnReut.clear();
    Unit unAval;

    //std::cout<<"************* INICIO Analise de atacks perdidos  **************"<<std::endl;

    //obtenho unidade que está sendo atacada  -- indice zero pois iremos obter a unica unidade inimiga na abstração.
    Unit & enemy = newState.getUnit(newState.getEnemy(_playerID), 0);
    //std::cout<<"Unidadade inimiga= "; enemy.print();

    HealthType enemyHP = enemy.currentHP();

    for (auto & mov : moveVec) {
        if (mov.type() == ActionTypes::ATTACK
                and enemy.ID() == state.getUnit(newState.getEnemy(_playerID), mov.index()).ID()
                ) {
            //std::cout<<"-- HP analisado= "<< enemyHP<< std::endl;
            //busco verificar se a unidade que foi atacada conseguiu ser eliminada sem necessitar de todas as ações do vetor
            //std::cout<<"-- Movimento de ataque= "<< mov.debugString()<< std::endl;
            //obtenho a unidade relacionada ao movimento
            unAval = state.getUnit(_playerID, mov.unit());
            if (enemyHP >= 0) {
                enemyHP = enemyHP - unAval.getDamageTo(enemy);
            } else {
                //std::cout<<"--- Ação considerada desnecessária. HP Atual= "<<enemyHP <<" "<< std::endl;
                //caso o Hp seja zero, esta será uma ação perdida. Removo do vetor.
                removeActionInVector(mov, moveVec);
                //removo do vetor _unAttack a unidade que não mais efetua ataques
                removeAttackInUnAttack(enemy, unAval);
                //adiciono a unidade aliada para nova abstração
                _UnReut.push_back(unAval);
            }
        }
    }


    //std::cout<<"************* FIM Analise de atacks perdidos  **************"<<std::endl;
}


//função utilizada para remoção das ações de dentro do vetor moveVec

void GenerationClassForMetricold::removeActionInVector(Action& action, std::vector<Action>& moveVec) {
    std::vector<Action> newMoveVec;
    for (auto & mov : moveVec) {
        if (!(mov == action)) {
            newMoveVec.push_back(mov);
        }
    }
    moveVec = newMoveVec;
}



//Esta função tem como objetivo definir e escolher a melhor unidade a ser atacada com base na abstração informada
//Comportamento inicial: Será criada a média da distância Euclidiana de todas as unidades aliadas em relação
//às unidades inimigas. Será escolhida a unidade inimiga que tiver a menor distância.

Unit& GenerationClassForMetricold::getCalculateEnemy(GameState& state, std::vector<Unit> unidadesInimigas) {
    std::map<Unit, PositionType> unDistance;
    std::map<Unit, PositionType>::iterator myIt;
    PositionType sum;
    //Utilziados na definição da melhor unidade
    Unit & bestUnit = unidadesInimigas[0];
    PositionType bestPosition = 999999;
    //std::cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<std::endl;
    //std::cout<<"Tamanho do vetor:"<< unidadesInimigas.size() <<std::endl;
    //std::cout<<"Unidade mais proxima no vetor:";
    //bestUnit.print();


    for (auto & un : unidadesInimigas) {
        sum = 0;
        for (int u(0); u < state.numUnits(_playerID); ++u) {
            sum += getDistEuclidiana(state.getUnit(_playerID, u).position(), un.position());
        }
        unDistance[un] = sum / state.numUnits(_playerID);
    }
    //seleciono como base de analise a unidade mais próxima
    //std::cout<<"__Listagem de unidades:"<<std::endl;
    for (myIt = unDistance.begin(); myIt != unDistance.end(); myIt++) {
        //std::cout<<"Distancia:"<<myIt->second<<"  ";
        //myIt->first.print();
        if (myIt->second < bestPosition) {
            bestPosition = myIt->second;
            bestUnit = myIt->first;
        }
    }

    //std::cout<<"Unidade mais proxima por distancia euclidiana:";
    //bestUnit.print();

    //aplico outras análises
    bestPosition = 9999999;
    for (myIt = unDistance.begin(); myIt != unDistance.end(); myIt++) {

        if (myIt->second < bestPosition
                and myIt->first.currentHP() < bestUnit.currentHP()
                and unitNeedMoreAttackForKilled(myIt->first)
                ) {
            bestPosition = myIt->second;
            bestUnit = myIt->first;
        }
    }
    //std::cout<<"Unidade mais proxima Escolhida:";
    //bestUnit.print();
    return bestUnit;

}

//função para rodar a abstração que será testada.
//entrada: Novo GameState com as unidades testadas. Vetor de ações que será retornado. GameState original.

void GenerationClassForMetricold::doAlphaBeta(GameState & newState, std::vector<Action> & moveVec, GameState & state) {
    //executa a busca
    alphaBeta->doSearch(newState);
    for (auto & mov : alphaBeta->getResults().bestMoves) {

        if (mov.type() == ActionTypes::ATTACK) {

            moveVec.push_back(Action(state.getIndexUnit(_playerID, newState.getUnit(_playerID, mov.unit()).ID()),
                    mov.player(),
                    mov.type(),
                    state.getIndexUnit(state.getEnemy(_playerID),
                    newState.getUnit(state.getEnemy(_playerID), mov.index()).ID()),
                    mov.pos()));
            //insere no vetor qual unidade aliada está atacando
            addAttack(newState.getUnit(state.getEnemy(_playerID), mov.index()), newState.getUnit(_playerID, mov.unit()));
        } else {
            moveVec.push_back(Action(state.getIndexUnit(_playerID, newState.getUnit(_playerID, mov.unit()).ID()),
                    mov.player(),
                    mov.type(),
                    mov.index(),
                    mov.pos()));
        }
    }
}

//adicionar uma unidade no vetor de controle de ataque de unidades.

void GenerationClassForMetricold::addAttack(const Unit& unitEnemy, const Unit& unitAttack) {
    if (_unAttack.find(unitEnemy) == _unAttack.end()) {
        //não foi encontrado. Insere no map
        std::vector<Unit> unitsAttack;
        unitsAttack.push_back(unitAttack);
        _unAttack[unitEnemy] = unitsAttack;
    } else {
        //apenas atualiza as unidades atacantes
        _unAttack.find(unitEnemy)->second.push_back(unitAttack);
    }
}

//utilizada para debug e verificação do map de controle de ataques.

void GenerationClassForMetricold::printMapAttack() {
    std::cout << " ********************************** " << std::endl;
    std::cout << " Relatório de unidades atacadas " << std::endl;
    for (std::map<Unit, std::vector<Unit> >::const_iterator it = _unAttack.begin(); it != _unAttack.end(); ++it) {
        std::cout << " Unidade atacada= " << std::endl;
        it->first.print();
        std::cout << " Unidades atacantes= " << std::endl;
        for (auto & un : it->second) {
            un.print();
        }
        std::cout << " " << std::endl;
    }
    std::cout << " ********************************** " << std::endl;
}

//Verifica qual a unidade válida para inclusão 

Unit GenerationClassForMetricold::getEnemyClosestvalid(GameState& state, std::vector<Unit> unidadesInimigas) {
    for (auto & un : unidadesInimigas) {
        if (!state.unitExist(un.player(), un.ID())) {
            if (unitNeedMoreAttackForKilled(un)) {
                return un;
            }
        }
    }
}

/*
 * Retorna todas as unidades de um player ordenados pela distância da unidade
 * passada como referência (inclusive a unidade passada como ponto de referencia)
 * levando em consideração as unidades que tem movimento válido
 */
void GenerationClassForMetricold::listaOrdenadaForMoves(const IDType& playerID, const Unit& unidade, GameState& state, std::vector<Unit>& unidades, const MoveArray& moves) {
    unidades.clear();
    //declaração
    Unit t;

    unidades.push_back(unidade);

    //obtenho posição atual da unidade base
    //Position currentPos = unidade.currentPosition(state.getTime());

    for (IDType u(0); u < moves.numUnits(); ++u) {
        t = state.getUnit(playerID, u);
        if (!unidade.equalsID(t)) {
            unidades.push_back(t);
            //size_t distSq(currentPos.getDistanceSq(t.currentPosition(state.getTime())));
            //size_t distSq(getDistManhantan(currentPos, t.currentPosition(state.getTime())) );
            //size_t distSq(getDistEuclidiana(currentPos, t.currentPosition(state.getTime())) );
        }
    }
    sortUnit(unidades, unidade, state);
}

void GenerationClassForMetricold::sortUnit(std::vector<Unit>& unidades, const Unit& base, GameState & state) {
    for (int i = 1; i < unidades.size(); i++) {
        Unit key = unidades[i];
        int j = i - 1;
        while ((j >= 0) && (getDistManhantan(base.currentPosition(state.getTime()), unidades[j].currentPosition(state.getTime()))
                > getDistManhantan(base.currentPosition(state.getTime()), key.currentPosition(state.getTime())))
                && (unidades[j].currentHP() >= key.currentHP())
                ) {
            unidades[j + 1] = unidades[j];
            j--;
        }
        unidades[j + 1] = key;
    }
}

/*
 * Retorna todas as unidades de um player ordenados pela distância da unidade
 * passada como referência (inclusive a unidade passada como ponto de referencia)
 */
void GenerationClassForMetricold::listaOrdenada(const IDType& playerID, const Unit & unidade, GameState& state, std::vector<Unit> & unidades) {
    unidades.clear();
    //declaração
    Unit t;

    //if(unidade.player() == playerID){
    //    unidades.push_back(unidade);
    //}

    //obtenho posição atual da unidade base
    Position currentPos = unidade.currentPosition(state.getTime());

    for (IDType u(0); u < state.numUnits(playerID); ++u) {
        t = state.getUnit(playerID, u);
        if (!unidade.equalsID(t)) {
            if (unitNeedMoreAttackForKilled(t)) {
                unidades.push_back(t);
            }
        }
    }
    sortUnit(unidades, unidade, state);
}

/*
 * Função tem o objetivo de realizar a cópia do state e já devolver ela 
 * com as unidades zeradas, mantando assim as outras informações 
 * necessárias.
 */
void GenerationClassForMetricold::copiarStateCleanUnit(GameState& origState, GameState& copState) {
    copState = origState;
    copState.cleanUpStateUnits();
}

//inicializa o player alpha beta com as configurações necessárias para executar
//os testes relacionados à classe

void GenerationClassForMetricold::iniciarAlphaBeta() {

    // convert them to the proper enum types
    int moveOrderingID = 1;
    int evalMethodID = 2; //testar com 1
    int playoutScriptID1 = 7;
    int playoutScriptID2 = 7;
    int playerToMoveID = 1;
    int opponentModelID = 7; //ultima opção oponente model script

    // construct the parameter object
    AlphaBetaSearchParameters params;

    // give the default parameters we can't set via options
    params.setMaxDepth(50);
    params.setSearchMethod(SearchMethods::IDAlphaBeta);

    // set the parameters from the options in the file
    params.setMaxPlayer(_playerID);
    params.setTimeLimit(40);
    params.setMaxChildren(0);
    params.setMoveOrderingMethod(moveOrderingID);
    params.setEvalMethod(evalMethodID);
    params.setSimScripts(playoutScriptID1, playoutScriptID2);
    params.setPlayerToMoveMethod(playerToMoveID);

    // add scripts for move ordering
    if (moveOrderingID == MoveOrderMethod::ScriptFirst) {
        params.addOrderedMoveScript(PlayerModels::NOKDPS);
        params.addOrderedMoveScript(PlayerModels::KiterDPS);
        //params.addOrderedMoveScript(PlayerModels::Cluster);
        //params.addOrderedMoveScript(PlayerModels::AttackWeakest);
    }

    // set opponent modeling if it's not none
    if (opponentModelID != PlayerModels::None) {
        if (_playerID == 0) {
            params.setSimScripts(playoutScriptID1, opponentModelID);
            params.setPlayerModel(1, playoutScriptID2);
        } else {
            params.setSimScripts(opponentModelID, playoutScriptID2);
            params.setPlayerModel(0, playoutScriptID1);
        }
    }

    //PlayerPtr abPlayer(new Player_AlphaBeta(_playerID, params, TTPtr((TranspositionTable *) NULL)));
    alphaBeta = new AlphaBetaSearchAbstract(params, TTPtr((TranspositionTable *) NULL));

}


//função para cálculo da distância baseada na fórmula de cálculo da Distância Manhantan

const PositionType GenerationClassForMetricold::getDistManhantan(const Position& pInicial, const Position& pFinal) {
    return abs(pInicial.x() - pFinal.x()) + abs(pInicial.y() - pFinal.y());
}

//função para cálculo da distância baseada na fórmua de cálculo utilizando a Distância Euclidiana

const PositionType GenerationClassForMetricold::getDistEuclidiana(const Position& pInicial, const Position& pFinal) {
    return sqrt(((pInicial.x() - pFinal.x())*(pInicial.x() - pFinal.x()) +
            (pInicial.y() - pFinal.y())*(pInicial.y() - pFinal.y())
            ));
}

//função utilizada para validar se existe a necessidade de mais um ataque para
//uma unidade inimiga ou se ela já irá morrer com os ataques existentes.

const bool GenerationClassForMetricold::unitNeedMoreAttackForKilled(Unit un) {
    if (_unAttack.find(un) == _unAttack.end()) {
        return true;
    }
    HealthType HpAtual = un.currentHP();
    for (auto & unAttack : _unAttack.find(un)->second) {
        HpAtual = HpAtual - unAttack.getDamageTo(un);
        if (HpAtual <= 0) {
            return false;
        }
    }

    return true;
}

//utilizada para remover um ataque da lista de atacantes de um determinado inimigo

void GenerationClassForMetricold::removeAttackInUnAttack(Unit enemy, Unit Attacker) {
    std::vector<Unit> cleanUnit;
    for (auto & unAttack : _unAttack.find(enemy)->second) {
        if (!(unAttack.ID() == Attacker.ID())) {
            cleanUnit.push_back(unAttack);
        }
    }
    _unAttack.find(enemy)->second = cleanUnit;
}

void GenerationClassForMetricold::iniciaAbstracao(GameState& state) {
    std::vector<std::string> combinacoes;
    
    gerarCombinacoes(combinacoes);
    
    std::vector<Unit> unTemp;
    for(auto & com : combinacoes){
        for(int i =0; i < com.length(); i++){
            char nova = com[i];
            int x = (int)nova - 48;
            unTemp.push_back(state.getUnit(_playerID, x ));
        }
        
        _vecAbstracao.push_back( copiaVector(unTemp));
        unTemp.clear();
    }
    
    /*std::cout<<"Mostrando todas as unidades="<<_vecAbstracao.size()<<std::endl;
    for(auto & units : _vecAbstracao){
        for(auto & un : units){
            un.print();
        }
        std::cout<<"-------"<<std::endl;
    }
     */
    
    
}

std::vector<Unit> GenerationClassForMetricold::copiaVector(std::vector<Unit> original){
    std::vector<Unit> novo;
    for(auto & un : original){
        novo.push_back(un);
    }
    return novo;
}

void GenerationClassForMetricold::printMedia(GameState& state){
    PositionType posicaoMedia(0);
    int qtd=0;
    std::set<Unit, lex_cm> newUnits;
    for(auto & un : _unitAbsAB){
        if(state.unitExist(_playerID, un.ID())){
            newUnits.insert( state.getUnitByID(_playerID, un.ID()) );
        }
    }
    
    std::vector<Unit> unidades;
    for(auto & un : newUnits){
        unidades.push_back(un);
    }
    
    for(int i = 0; i < (unidades.size()-1); i++){
        //std::cout<<"Unidade Base: ";
        // unidades[i].print();
        for(int j = i+1; j < unidades.size(); j++){
            //std::cout<<"____Unidade secundaria: ";
            //unidades[j].print();
            posicaoMedia += getDistEuclidiana( unidades[i].currentPosition(state.getTime()), unidades[j].currentPosition(state.getTime()) );
            qtd++;
        }
         //std::cout<<" "<<std::endl;
    }
    
    double media = ( (double)posicaoMedia) / ((double)qtd);
    std::cout<<"Distancia Total = "<< posicaoMedia <<" qtd= "<<qtd<<std::endl;
    if(qtd == 0){
        std::cout<<"Distancia Media = "<< 0 <<std::endl;
    }else{
        std::cout<<"Distancia Media = "<< media <<std::endl;
    }
    std::cout<<"-------------- FIM DISTANCIAS ---------------------- "<<std::endl;
}

void GenerationClassForMetricold::gerarCombinacoes(std::vector<std::string>& combinacoes){
    unsigned MAX, MASK, NUM ;
    int i, j, r, k ;
    /* Armazena a string de entrada. */
    char input[31] ;
    /* Armazena cada combinação. */
    char str[31] ;

    strcpy(input, "0123456789");
    r = 4;

    
    MAX = ~(1 << strlen(input)) ;

    /* Primeiro número é o 1. */
    NUM = 1;

    /* Quando o número alcançar MAX, o loop
     * será encerrado.
     */
    while ( MAX & NUM ) {
        /* Conta os bits 1's. */
        MASK = 1 ;
        k = 0 ;
        while ( MAX & MASK ) {
            if ( NUM & MASK ) k++ ;
            MASK = MASK << 1 ;
        }

        /* Monta o resultado somente se
         * a quantidade de bits k é igual
         * a r. */
        if ( k == r ) {
            MASK = 1 ;
            i = j = 0 ;

            while ( MAX & MASK ) {
                /* Verdadeiro se NUM tem um bit 1
                 * na posição indicada por MASK. */
                if ( NUM & MASK ) {
                    /* Gera a combinação em str */
                    str[i] = input[j] ;
                    i++ ;
                }
                j++ ;
                /* Desloca a máscara */
                MASK = MASK << 1 ;
            }

            str[i]=0 ;
            combinacoes.push_back(str);
        }

        NUM++ ;
    }
    
}
