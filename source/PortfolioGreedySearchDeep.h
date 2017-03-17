#pragma once

#include "Common.h"
#include "GameState.h"
#include "Player.h"
#include "Game.h"
#include "Action.h"
#include "UnitScriptData.h"
#include <memory>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/stat.h>
#include "MetricDeep.h"

namespace SparCraft
{
	
typedef	std::shared_ptr<Player> PlayerPtr;

class PortfolioGreedySearchDeep
{
protected:
	
    const IDType		_player;
    const IDType		_enemyScript;
    const size_t		_iterations;
    const size_t                _responses;
    std::vector<IDType>		_playerScriptPortfolio;
    size_t                      _totalEvals;
    size_t                      _timeLimit;
    std::ofstream		_fileTime;
    
    int                         controlPartidas;

    void                        doPortfolioSearch(const IDType & player,const GameState & state,UnitScriptData & currentData, Timer & t);
    std::vector<Action>     	getMoveVec(const IDType & player,const GameState & state,const std::vector<IDType> & playerScripts);
    StateEvalScore              eval(const IDType & player,const GameState & state,UnitScriptData & playerScriptsChosen);
    IDType                      calculateInitialSeed(const IDType & player,const GameState & state);
    void                        setAllScripts(const IDType & player,const GameState & state,UnitScriptData & data,const IDType & script);

public:

    PortfolioGreedySearchDeep(const IDType & player, const IDType & enemyScript, const size_t & iter, const size_t & responses, const size_t & timeLimit, int numPartida);
    std::vector<Action> search(const IDType & player, const GameState & state);
    UnitScriptData searchForScripts(const IDType & player, const GameState & state);
    void saveMetrics(MetricDeep & metrica, std::vector<Action>& moveVec);
};

}
