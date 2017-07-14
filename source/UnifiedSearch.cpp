#include "UnifiedSearch.h"


using namespace SparCraft;

UnifiedSearch::UnifiedSearch(IDType playerID, GameState & state) {
    _playerID = playerID;

}

UnifiedSearch::~UnifiedSearch() {
}

/*
    std::cout << "------------- Movimentos ---------------"<<std::endl;
    allMoves.print();
    std::cout << "------------- Movimentos ---------------"<<std::endl;
    std::cout << "------------- unidades controladas ---------------"<<std::endl;
    for (auto un : unitsAB) {
        std::cout << (int)un << std::endl;
    }

    std::cout << "------------- unidades controladas ---------------"<<std::endl;
 */



std::vector<Action> UnifiedSearch::doSearchWithMoves(GameState& state, UnitScriptData& UnitScriptData, std::set<IDType>& unitsAB, size_t timeLimit) {
    Timer t;
    t.start();
    // faço a geração dos movimentos baseados no UnitScriptData 
    MoveArray moves;
    state.generateMoves(moves, _playerID);
    std::vector<Action> moveVec;
    GameState copy(state);
    UnitScriptData.calculateMoves(_playerID, moves, copy, moveVec);

    //controle do melhor 
    StateEvalScore bestEval = eval(moveVec, state);
    std::vector<Action> bestMoveVec = moveVec;

    //preciso obter os movimentos de todas as unidades para trocar as controladas
    MoveArray allMoves;
    state.generateMoves(allMoves, _playerID);

    //efetuo a troca dentro do MoveArray
    while (timeLimit < t.getElapsedTimeInMilliSec()) {
        for (size_t unitIndex(0); unitIndex < moves.numUnits(); ++unitIndex) {
            const Unit & unit = state.getUnit(_playerID, unitIndex);

            if (unitsAB.find(unit.ID()) != unitsAB.cend()) {
                std::vector<Action> movesTemp = allMoves.getMoves(unitIndex);

                for (auto action : movesTemp) {
                    std::vector<Action> moveVecTemp = bestMoveVec;
                    moveVecTemp[action.unit()] = action;
                    StateEvalScore newEval = eval(moveVecTemp, state);
                    if (newEval > bestEval) {
                        bestEval = newEval;
                        bestMoveVec = moveVecTemp;
                    }
                }

            }
        }
    }

    return bestMoveVec;
}

StateEvalScore UnifiedSearch::eval(std::vector<Action> moveVec, GameState& state) {

    UnitScriptData _baseScriptData;
    const IDType enemyPlayer(state.getEnemy(_playerID));
    //inicializar o baseScriptData com NO-KDPS
    //Player
    for (size_t unitIndex(0); unitIndex < state.numUnits(_playerID); ++unitIndex) {
        _baseScriptData.setUnitScript(state.getUnit(_playerID, unitIndex), SparCraft::PlayerModels::NOKDPS);
    }
    //Enemy
    for (size_t unitIndex(0); unitIndex < state.numUnits(enemyPlayer); ++unitIndex) {
        _baseScriptData.setUnitScript(state.getUnit(enemyPlayer, unitIndex), SparCraft::PlayerModels::NOKDPS);
    }

    std::vector<Action> moveVecPgsEnemy;
    if (state.bothCanMove()) {
        //gero os movimentos inimigos
        MoveArray movesPGSEnemy;
        state.generateMoves(movesPGSEnemy, enemyPlayer);
        GameState copy2(state);
        _baseScriptData.calculateMoves(enemyPlayer, movesPGSEnemy, copy2, moveVecPgsEnemy);
    }

    //Execução AB-PGS
    Game gABPGS(state, 25);
    gABPGS.getState().makeMoves(moveVec);
    if (gABPGS.getState().bothCanMove()) {
        gABPGS.getState().makeMoves(moveVecPgsEnemy);
    }
    gABPGS.getState().finishedMoving();
    gABPGS.playIndividualScripts(_baseScriptData);

    return gABPGS.getState().eval(_playerID, SparCraft::EvaluationMethods::LTD2);
}



