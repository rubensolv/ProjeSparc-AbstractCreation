#include "Player_AdaptableStratifiedPolicySearchLimit.h"

using namespace SparCraft;

Player_AdaptableStratifiedPolicySearchLimit::Player_AdaptableStratifiedPolicySearchLimit (const IDType & playerID)
{
	_playerID = playerID;
	_iterations = 1;
    _responses = 0;
	_seed = PlayerModels::NOKDPS;
}

Player_AdaptableStratifiedPolicySearchLimit::Player_AdaptableStratifiedPolicySearchLimit (const IDType & playerID, const IDType & seed, const size_t & iter, const size_t & responses, const size_t & timeLimit)
{
	_playerID = playerID;
	_iterations = iter;
    _responses = responses;
	_seed = seed;
    _timeLimit = timeLimit;
}

void Player_AdaptableStratifiedPolicySearchLimit::getMoves(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec)
{
    moveVec.clear();
    StateEvalScore SSSScore;
    AdaptableStratifiedPolicySearchLimit pgs(_playerID, _seed, _iterations, _responses, _timeLimit);

    moveVec = pgs.search(_playerID, state, SSSScore);
}
