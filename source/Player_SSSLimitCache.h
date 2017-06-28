#pragma once

#include "Common.h"
#include "Player.h"
#include "SSSLimitCache.h"

namespace SparCraft
{
class Player_SSSLimitCache : public Player
{
	IDType _seed;
	size_t _iterations;
    size_t _responses;
    size_t _timeLimit;
public:
    Player_SSSLimitCache (const IDType & playerID);
    Player_SSSLimitCache (const IDType & playerID, const IDType & seed, const size_t & iter, const size_t & responses, const size_t & timeLimit);
	void getMoves(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec);
    IDType getType() { return PlayerModels::AdaptableStratifiedPolicySearchLimit; }
};
}
