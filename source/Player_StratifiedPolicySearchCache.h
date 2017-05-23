#pragma once

#include "Common.h"
#include "Player.h"
#include "StratifiedPolicySearchCache.h"

namespace SparCraft
{
class Player_StratifiedPolicySearchCache : public Player
{
	IDType _seed;
	size_t _iterations;
    size_t _responses;
    size_t _timeLimit;
    size_t _hpLevelDiv;
public:
    Player_StratifiedPolicySearchCache (const IDType & playerID);
    Player_StratifiedPolicySearchCache (const IDType & playerID, const IDType & seed, const size_t & iter, const size_t & responses, const size_t & timeLimit, const size_t & hpLevelDiv);
	void getMoves(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec);
    IDType getType() { return PlayerModels::StratifiedPolicySearchCache; }
};
}
