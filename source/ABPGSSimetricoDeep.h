#pragma once

/* 
 * File:   GenerationClass.h
 * Author: rubens
 *
 * Created on 19 de Novembro de 2016, 14:35
 */

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/stat.h>
#include "Player.h"
#include "AlphaBetaSearchParameters.hpp"
#include "AlphaBetaSearchSimetric.h"
#include "PortfolioGreedySearchNoTime.h"
#include "MetricDeep.h"

class TranspositionTable;

namespace SparCraft {

    class AlphaBetaSearchSimetric;
    class PortfolioGreedySearchNoTime;
    
    struct lex_simDeep {

        bool operator()(const Unit & lUn, const Unit & rUn) const {
            return lUn < rUn;
        }
    };
    

    class ABPGSSimetricoDeep : public Player {
        AlphaBetaSearchSimetric * alphaBeta;
        PortfolioGreedySearchNoTime * pgs;
        std::map<Unit, std::vector<Unit>> _unAttack;
        std::vector<Unit> _UnReut;
        std::set<Unit, lex_simDeep> _unitAbsAB;
        TimeType lastTime;
        int numUnits;
        int controlPartidas;
    public:
        ABPGSSimetricoDeep(const IDType & playerID);
        ABPGSSimetricoDeep(const IDType & playerID, int numUnitsAB);
        void getMoves(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec);

        IDType getType() {
            return PlayerModels::ABPGSSimetricoDeep;
        }
        void listaOrdenada(const IDType & playerID, const Unit & unidade, GameState & state, std::vector<Unit> & unidades);
        void listaOrdenadaForMoves(const IDType & playerID, const Unit & unidade, GameState & state, std::vector<Unit> & unidades, const MoveArray & moves);
        void copiarStateCleanUnit(GameState & origState, GameState & copState);
        void iniciarAlphaBeta();
        void saveMetrics(MetricDeep & metrica, std::vector<Action>& moveVec);
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
        
        //controla a inicialição das abstrações para métrica
        std::vector<Unit> copiaVector(std::vector<Unit> original);
        void calculateMedia(GameState& state, MetricDeep& metric);
    };
}


