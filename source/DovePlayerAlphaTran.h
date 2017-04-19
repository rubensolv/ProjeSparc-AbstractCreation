#pragma once

#include <stdlib.h>
#include "Player.h"
#include "PortfolioGreedySearchCache.h"
#include "PortfolioOnlineEvolutionCache.h"

namespace SparCraft {
    
    class PortfolioGreedySearchCache;
    class PortfolioOnlineEvolutionCache;
    class PortfolioOnlineGenome;

    class DovePlayerAlphaTran : public Player {
        PortfolioGreedySearchCache * pgs;
        PortfolioOnlineEvolutionCache * poe;
    public:
        DovePlayerAlphaTran(const IDType & playerID);
        virtual ~DovePlayerAlphaTran();
        void getMoves(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec);
    private:

    };
}

