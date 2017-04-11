#pragma once

#include <stdlib.h>
#include "Player.h"
#include "PortfolioGreedySearchCache.h"
#include "PortfolioOnlineEvolutionCache.h"

namespace SparCraft {
    
    class PortfolioGreedySearchCache;
    class PortfolioOnlineEvolutionCache;
    class PortfolioOnlineGenome;

    class DovePlayerAlpha : public Player {
        PortfolioGreedySearchCache * pgs;
        PortfolioOnlineEvolutionCache * poe;
    public:
        DovePlayerAlpha(const IDType & playerID);
        virtual ~DovePlayerAlpha();
        void getMoves(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec);
    private:

    };
}

