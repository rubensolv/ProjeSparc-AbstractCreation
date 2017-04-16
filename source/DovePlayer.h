#pragma once

#include <stdlib.h>
#include <cmath>
#include "Player.h"
#include "PortfolioGreedySearchCache.h"
#include "PortfolioOnlineEvolutionCache.h"

namespace SparCraft {
    
    class PortfolioGreedySearchCache;
    class PortfolioOnlineEvolutionCache;
    class PortfolioOnlineGenome;

    class DovePlayerBeta : public Player {
        PortfolioGreedySearchCache * pgs;
        PortfolioOnlineEvolutionCache * poe;
    public:
        double _costTimePlayout;
        DovePlayerBeta(const IDType & playerID);
        virtual ~DovePlayerBeta();
        void getMoves(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec);
    private:
        bool canApplyExaustSearch(GameState & state);
        void executeDovetailing(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec);
        void executeExaustSearch(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec);
        int proxima(int a[], int N, int M);
        std::vector<UnitScriptData> obtemSeqUnitScriptData(GameState& state,int n, int m);
        StateEvalScore playUnitScripts(GameState& state, UnitScriptData script); 
    };
}

