#include "Player_PortfolioGreedySearchDeep.h"

using namespace SparCraft;

Player_PortfolioGreedySearchDeep::Player_PortfolioGreedySearchDeep (const IDType & playerID) 
{
	_playerID = playerID;
	_iterations = 1;
    _responses = 0;
	_seed = PlayerModels::NOKDPS;
        lastTime = 0;
        controlPartidas = 1;
}

Player_PortfolioGreedySearchDeep::Player_PortfolioGreedySearchDeep (const IDType & playerID, const IDType & seed, const size_t & iter, const size_t & responses, const size_t & timeLimit)
{
	_playerID = playerID;
	_iterations = iter;
    _responses = responses;
	_seed = seed;
    _timeLimit = timeLimit;
    lastTime = 0;
    controlPartidas = 1;
}

void Player_PortfolioGreedySearchDeep::getMoves(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec)
{
    if (lastTime > state.getTime()) {
        controlPartidas++;
    }
    lastTime = state.getTime();
    
    moveVec.clear();
	PortfolioGreedySearchDeep pgs(_playerID, _seed, _iterations, _responses, _timeLimit, controlPartidas);

	moveVec = pgs.search(_playerID, state);
}

