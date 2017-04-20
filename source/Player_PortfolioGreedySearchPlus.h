#pragma once

#include "Common.h"
#include "Player.h"
#include "PortfolioGreedySearchPlus.h"

namespace SparCraft
{
class Player_PortfolioGreedySearchPlus : public Player
{
	IDType _seed;
	size_t _iterations;
    size_t _responses;
    size_t _timeLimit;
public:
	Player_PortfolioGreedySearchPlus (const IDType & playerID);
    Player_PortfolioGreedySearchPlus (const IDType & playerID, const IDType & seed, const size_t & iter, const size_t & responses, const size_t & timeLimit);
	void getMoves(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec);
    IDType getType() { return PlayerModels::PortfolioGreedySearch; }
};
}