#include "Player_StratifiedPolicySearchCache.h"

using namespace SparCraft;

Player_StratifiedPolicySearchCache::Player_StratifiedPolicySearchCache (const IDType & playerID)
{
	_playerID = playerID;
	_iterations = 1;
    _responses = 0;
	_seed = PlayerModels::NOKDPS;
}

Player_StratifiedPolicySearchCache::Player_StratifiedPolicySearchCache (const IDType & playerID, const IDType & seed, const size_t & iter, const size_t & responses, const size_t & timeLimit, const size_t & hpLevelDiv)
{
	_playerID = playerID;
	_iterations = iter;
    _responses = responses;
	_seed = seed;
    _timeLimit = timeLimit;
    _hpLevelDiv = hpLevelDiv;
}

void Player_StratifiedPolicySearchCache::getMoves(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec)
{
    moveVec.clear();
	StratifiedPolicySearchCache pgs(_playerID, _seed, _iterations, _responses, _timeLimit, _hpLevelDiv);

	moveVec = pgs.search(_playerID, state);
}
