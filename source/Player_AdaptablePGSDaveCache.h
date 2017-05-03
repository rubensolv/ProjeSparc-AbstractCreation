#pragma once

#include "Common.h"
#include "Player.h"
#include "AdaptablePGSPlusDaveCache.h"

namespace SparCraft {
    class AdaptablePGSPlusDaveCache;

    class Player_AdaptablePGSDaveCache : public Player {
        IDType _seed;
        size_t _iterations;
        size_t _responses;
        size_t _timeLimit;
        //for adaptable playout steps
        int _numSteps;
        TimeType _lastTimeState;
    public:
        Player_AdaptablePGSDaveCache(const IDType & playerID);
        Player_AdaptablePGSDaveCache(const IDType & playerID, const IDType & seed, const size_t & iter, const size_t & responses, const size_t & timeLimit);
        void getMoves(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec);

        IDType getType() {
            return PlayerModels::PortfolioGreedySearch;
        }
        
    protected:
        void updateInfoPGS(AdaptablePGSPlusDaveCache & pgs, GameState & state);
    };
}