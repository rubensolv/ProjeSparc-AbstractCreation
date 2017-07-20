#pragma once

/* 
 * File:   GenerationClass.h
 * Author: rubens
 *
 * Created on 19 de Novembro de 2016, 14:35
 */

#include <stdlib.h>
#include "Player.h"
#include "AlphaBetaSearchParameters.hpp"
#include "AlphaBetaSearchAbstractSAB.h"
#include "AdaptableStratifiedPolicySearchLimit.h"
#include "ManagerAbstraction.h"
#include "ManagerRandom.h"
#include "ManagerClosest.h"
#include "ManagerFarther.h"
#include "ManagerLessLife.h"
#include "ManagerMoreLife.h"
#include "ManagerClosestEnemy.h"
#include "ManagerFartherEnemy.h"
#include "ManagerMoreDPS.h"
#include "ManagerLessDPS.h"
#include "UnifiedSearch.h"

class TranspositionTable;

namespace SparCraft {

    class AlphaBetaSearchAbstractSAB;
    class AdaptableStratifiedPolicySearchLimit;   
    class CacheSimpleString;    
    class UnifiedSearch;

    class SUS : public Player {
        AlphaBetaSearchAbstractSAB * alphaBeta;
        AdaptableStratifiedPolicySearchLimit * pgs;
        UnifiedSearch * us;
        std::map<Unit, std::vector<Unit>> _unAttack;
        std::vector<Unit> _UnReut;
        std::set<Unit> _unitAbsAB;
        TimeType lastTime;
        int numUnits;
        ManagerAbstraction * manager;
    public:
        SUS(const IDType & playerID);
        SUS(const IDType & playerID, int numUnitsAB, std::string controlAbstraction);
        ~SUS();
        void getMoves(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec);

        IDType getType() {
            return PlayerModels::SUS;
        }
        void listaOrdenada(const IDType & playerID, const Unit & unidade, GameState & state, std::vector<Unit> & unidades);
        void listaOrdenadaForMoves(const IDType & playerID, const Unit & unidade, GameState & state, std::vector<Unit> & unidades, const MoveArray & moves);
        void copiarStateCleanUnit(GameState & origState, GameState & copState);
        void iniciarAlphaBeta();
    private:
        Unit getEnemyClosestvalid(GameState & state, std::vector<Unit> unidadesInimigas);
        //manipulação do controle de atacantes
        void addAttack(const Unit & unitEnemy, const Unit & unitAttack);
        void printMapAttack();
        const PositionType getDistManhantan(const Position & pInicial, const Position & pFinal);
        const PositionType getDistEuclidiana(const Position & pInicial, const Position & pFinal);
        const bool unitNeedMoreAttackForKilled(Unit un);
        void doAlphaBeta(GameState & newState, std::vector<Action> & moveVec, GameState& state);
        void sortUnit(std::vector<Unit> & unidades, const Unit & unit, GameState & state);
        Unit& getCalculateEnemy(GameState & state, std::vector<Unit> unidadesInimigas);
        void removeLoseAttacks(GameState & newState, std::vector<Action> & moveVec, GameState & state);
        void removeActionInVector(Action & action, std::vector<Action>& moveVec);
        void removeAttackInUnAttack(Unit enemy, Unit Attacker);
        void controlUnitsForAB(GameState & state, const MoveArray & moves);
        bool unitsInMoves(GameState& state, const MoveArray& moves);


        //idéia da análise de inclusão de outras unidades inimigas que podem ser atacadas
        void addMoreEnemy(GameState & newState, std::vector<Unit> & unInimigas);

        //idéia da movimentação por inicialiaçao
        bool applyClosestInicialization(std::vector<Unit> & unAliadas, std::vector<Unit> & unInimigas, GameState & state);

        //ideia de analisar as ações dada à uma unidade
        void analisarAbstractForm(GameState newState, std::vector<Unit> unidadesInimigas);
        
        //métodos utilização das classes de gestão de unidades
        void iniciarClasseAbstracao(std::string controlAbstraction);
        
        //método temp para comparar jogadas
        StateEvalScore eval(std::vector<Action> moveVec, GameState& state);
        
    };
}


