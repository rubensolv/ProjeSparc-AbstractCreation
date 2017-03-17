#include "Player_PortfolioOnlineEvolutionDeep.h"

using namespace SparCraft;

Player_PortfolioOnlineEvolutionDeep::Player_PortfolioOnlineEvolutionDeep (const IDType & playerID)
{
	_playerID = playerID;
	_iterations = 1;
    _responses = 0;
	_seed = PlayerModels::NOKDPS;
        lastTime = 0;
        controlPartidas = 1;
}

Player_PortfolioOnlineEvolutionDeep::Player_PortfolioOnlineEvolutionDeep (const IDType & playerID, const IDType & seed, const size_t & iter, const size_t & responses, const size_t & timeLimit)
{
	_playerID = playerID;
	_iterations = iter;
    _responses = responses;
	_seed = seed;
    _timeLimit = timeLimit;
    lastTime = 0;
    controlPartidas = 1;
}

void Player_PortfolioOnlineEvolutionDeep::getMoves(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec)
{
    if (lastTime > state.getTime()) {
        controlPartidas++;
    }
    lastTime = state.getTime();
    
    moveVec.clear();
	PortfolioOnlineEvolutionDeep pgs(_playerID, _seed, _iterations, _responses, _timeLimit, controlPartidas);

	moveVec = pgs.search(_playerID, state);
}
