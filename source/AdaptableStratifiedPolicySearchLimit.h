#pragma once

#include "Common.h"
#include "GameState.h"
#include "Player.h"
#include "Game.h"
#include "Action.h"
#include "UnitScriptData.h"
#include <memory>
#include "AdaptableStratType.h"
#include <fstream>

namespace SparCraft
{
	
typedef	std::shared_ptr<Player> PlayerPtr;

class AdaptableStratifiedPolicySearchLimit
{
protected:
	
    const IDType				_player;
    const IDType				_enemyScript;
    const size_t				_iterations;
    const size_t                _responses;
    std::vector<IDType>			_playerScriptPortfolio;
    size_t                      _totalEvals;
    size_t                      _timeLimit;
    std::ofstream				_fileTime;

    bool                        doStratifiedSearch(const IDType & player,const GameState & state,UnitScriptData & currentData, Timer & timer, int & numberTypes, double & timePlayouts , StateEvalScore & bestScore);
    std::vector<Action>     	getMoveVec(const IDType & player,const GameState & state,const std::vector<IDType> & playerScripts);
    StateEvalScore              eval(const IDType & player,const GameState & state,UnitScriptData & playerScriptsChosen);
    IDType                      calculateInitialSeed(const IDType & player,const GameState & state);
    void                        setAllScripts(const IDType & player,const GameState & state,UnitScriptData & data,const IDType & script);

public:

    AdaptableStratifiedPolicySearchLimit(const IDType & player, const IDType & enemyScript, const size_t & iter, const size_t & responses, const size_t & timeLimit);
    std::vector<Action> search(const IDType & player, const GameState & state, StateEvalScore & bestScore);
    UnitScriptData searchForScripts(const IDType & player, const GameState & state, StateEvalScore & bestScore);
};

}
