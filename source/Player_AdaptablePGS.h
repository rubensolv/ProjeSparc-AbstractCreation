#pragma once

#include "Common.h"
#include "Player.h"
#include "AdaptablePGSPlus.h"

namespace SparCraft {
    class AdaptablePGSPlus;

    class Player_AdaptablePGS : public Player {
        IDType _seed;
        size_t _iterations;
        size_t _responses;
        size_t _timeLimit;
        //for adaptable playout steps
        int _numSteps;
        TimeType _lastTimeState;
    public:
        Player_AdaptablePGS(const IDType & playerID);
        Player_AdaptablePGS(const IDType & playerID, const IDType & seed, const size_t & iter, const size_t & responses, const size_t & timeLimit);
        void getMoves(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec);

        IDType getType() {
            return PlayerModels::PortfolioGreedySearch;
        }
        
    protected:
        void updateInfoPGS(AdaptablePGSPlus & pgs, GameState & state);
    };
}