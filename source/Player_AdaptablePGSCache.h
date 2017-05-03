#pragma once

#include "Common.h"
#include "Player.h"
#include "AdaptablePGSPlusCache.h"

namespace SparCraft {
    class AdaptablePGSPlusCache;

    class Player_AdaptablePGSCache : public Player {
        IDType _seed;
        size_t _iterations;
        size_t _responses;
        size_t _timeLimit;
        //for adaptable playout steps
        int _numSteps;
        TimeType _lastTimeState;
    public:
        Player_AdaptablePGSCache(const IDType & playerID);
        Player_AdaptablePGSCache(const IDType & playerID, const IDType & seed, const size_t & iter, const size_t & responses, const size_t & timeLimit);
        void getMoves(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec);

        IDType getType() {
            return PlayerModels::PortfolioGreedySearch;
        }
        
    protected:
        void updateInfoPGS(AdaptablePGSPlusCache & pgs, GameState & state);
    };
}