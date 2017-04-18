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

    class DovePlayer : public Player {
        PortfolioGreedySearchCache * pgs;
        PortfolioOnlineEvolutionCache * poe;
        std::string _useScriptPGS;
    public:
        double _costTimePlayout;
        DovePlayer(const IDType & playerID, std::string useScriptPGS);
        virtual ~DovePlayer();
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

