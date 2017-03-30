#pragma once

/* 
 * File:   GenerationClass.h
 * Author: rubens
 *
 * Created on 19 de Novembro de 2016, 14:35
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "Player.h"
#include "AlphaBetaSearchParameters.hpp"
#include "AlphaBetaSearchAbstract.h"
#include "PortfolioGreedySearchNoTime.h"
#include "MetricGAB.h"

class TranspositionTable;

namespace SparCraft {

    class AlphaBetaSearchAbstract;
    class PortfolioGreedySearchNoTime;
    
    struct lex_mets {

        bool operator()(const Unit & lUn, const Unit & rUn) const {
            return lUn < rUn;
        }
    };
    

    class GenerationClassForMetricSample : public Player {
        AlphaBetaSearchAbstract * alphaBeta;
        PortfolioGreedySearchNoTime * pgs;
        std::map<Unit, std::vector<Unit>> _unAttack;
        std::vector<Unit> _UnReut;
        std::set<Unit, lex_mets> _unitAbsAB;
        TimeType lastTime;
	std::vector< std::vector<Unit> > _vecAbstracao;
        int _controlNumAbs;
        int numUnits;
    public:
        GenerationClassForMetricSample(const IDType & playerID);
        GenerationClassForMetricSample(const IDType & playerID, int numUnitsAB);
        void getMoves(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec);

        IDType getType() {
            return PlayerModels::GenerationClassForMetricSample;
        }
        void listaOrdenada(const IDType & playerID, const Unit & unidade, GameState & state, std::vector<Unit> & unidades);
        void listaOrdenadaForMoves(const IDType & playerID, const Unit & unidade, GameState & state, std::vector<Unit> & unidades, const MoveArray & moves);
        void copiarStateCleanUnit(GameState & origState, GameState & copState);
        void iniciarAlphaBeta();
        void saveMetrics(MetricGAB & metrica);
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
        void iniciaAbstracao(GameState& state);
        std::vector<Unit> copiaVector(std::vector<Unit> original);
        void calculateMedia(GameState& state, MetricGAB& metric);
        void lerAmostragem(std::vector<std::vector<int> > & combinacoes);
        void split(const std::string &s, std::vector<std::string> & v);
        
    };
}


