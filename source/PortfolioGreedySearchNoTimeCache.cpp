#include "PortfolioGreedySearchNoTimeCache.h"

using namespace SparCraft;

PortfolioGreedySearchNoTimeCache::PortfolioGreedySearchNoTimeCache(const IDType & player, const IDType & enemyScript, const size_t & iter, const size_t & responses, const size_t & timeLimit)
	: _player(player)
	, _enemyScript(enemyScript)
	, _iterations(iter)
    , _responses(responses)
    , _totalEvals(0)
    , _timeLimit(timeLimit)
{
	_playerScriptPortfolio.push_back(PlayerModels::NOKDPS);
	_playerScriptPortfolio.push_back(PlayerModels::KiterDPS);
	//_playerScriptPortfolio.push_back(PlayerModels::Cluster);
        
}

PortfolioGreedySearchNoTimeCache::~PortfolioGreedySearchNoTimeCache() {
    
}


UnitScriptData PortfolioGreedySearchNoTimeCache::searchForScripts(const IDType & player, const GameState & state, StateEvalScore & bestScore)
{
    cacheLTD2 = new CacheSimpleString();
    _qtdPlayoutIgnorar = 0;
    
    Timer t;
    t.start();

    const IDType enemyPlayer(state.getEnemy(player));

    // calculate the seed scripts for each player
    // they will be used to seed the initial root search
    IDType seedScript = calculateInitialSeed(player, state);
    IDType enemySeedScript = calculateInitialSeed(enemyPlayer, state);

    // set up the root script data
    UnitScriptData originalScriptData;
    setAllScripts(player, state, originalScriptData, seedScript);
    setAllScripts(enemyPlayer, state, originalScriptData, enemySeedScript);

    double ms = t.getElapsedTimeInMilliSec();
    //printf("\nFirst Part %lf ms\n", ms);

    // do the initial root portfolio search for our player
    UnitScriptData currentScriptData(originalScriptData);
    doPortfolioSearch(player, state, currentScriptData, t, bestScore);
/*
    // iterate as many times as required
    for (size_t i(0); i<_responses; ++i)
    {
        // do the portfolio search to improve the enemy's scripts
        doPortfolioSearch(enemyPlayer, state, currentScriptData,t);

        // then do portfolio search again for us to improve vs. enemy's update
        doPortfolioSearch(player, state, currentScriptData, t);
    }
*/
    // convert the script vector into a move vector and return it
	//MoveArray moves;
	//state.generateMoves(moves, player);
    //std::vector<Action> moveVec;
    //GameState copy(state);
    //currentScriptData.calculateMoves(player, moves, copy, moveVec);
    
    _totalEvals = 0;
    free(cacheLTD2);
    
    return  currentScriptData;
}

std::vector<Action> PortfolioGreedySearchNoTimeCache::search(const IDType & player, const GameState & state, StateEvalScore & bestScore)
{
    Timer t;
    t.start();

    const IDType enemyPlayer(state.getEnemy(player));

    // calculate the seed scripts for each player
    // they will be used to seed the initial root search
    IDType seedScript = calculateInitialSeed(player, state);
    IDType enemySeedScript = calculateInitialSeed(enemyPlayer, state);

    // set up the root script data
    UnitScriptData originalScriptData;
    setAllScripts(player, state, originalScriptData, seedScript);
    setAllScripts(enemyPlayer, state, originalScriptData, enemySeedScript);

    double ms = t.getElapsedTimeInMilliSec();
    //printf("\nFirst Part %lf ms\n", ms);

    // do the initial root portfolio search for our player
    UnitScriptData currentScriptData(originalScriptData);
    doPortfolioSearch(player, state, currentScriptData, t, bestScore);

    // iterate as many times as required
    for (size_t i(0); i<_responses; ++i)
    {
        // do the portfolio search to improve the enemy's scripts
        doPortfolioSearch(enemyPlayer, state, currentScriptData, t, bestScore);

        // then do portfolio search again for us to improve vs. enemy's update
        doPortfolioSearch(player, state, currentScriptData, t, bestScore);
    }

    ms = t.getElapsedTimeInMilliSec();
/*
    _fileTime.open("PGS.txt", std::ostream::app);
    if (!_fileTime.is_open())
    {
    	std::cout << "ERROR Opening file" << std::endl;
    }
    _fileTime << ms << ", ";
    _fileTime.close();
*/
    // convert the script vector into a move vector and return it
	MoveArray moves;
	state.generateMoves(moves, player);
    std::vector<Action> moveVec;
    GameState copy(state);
    currentScriptData.calculateMoves(player, moves, copy, moveVec);

    ms = t.getElapsedTimeInMilliSec();
    //printf("\nMove PGS chosen in %lf ms\n", ms);

    _totalEvals = 0;

    return moveVec;
}

void PortfolioGreedySearchNoTimeCache::doPortfolioSearch(const IDType & player, const GameState & state, UnitScriptData & currentScriptData, Timer & t, StateEvalScore & bestScore)
{
  //  Timer t;
 //   t.start();

    // the enemy of this player
    const IDType enemyPlayer(state.getEnemy(player));

    int counterIterations = 0;

    while(t.getElapsedTimeInMilliSec() < _timeLimit)
    //for (size_t i(0); i<_iterations; ++i)
    {
        // set up data for best scripts
        IDType          bestScriptVec[Constants::Max_Units];
	    StateEvalScore  bestScoreVec[Constants::Max_Units];
	    bool hasImproved = false;

        // for each unit that can move
        for (size_t unitIndex(0); unitIndex<state.numUnits(player); ++unitIndex)
        {
            if (_timeLimit > 0 && t.getElapsedTimeInMilliSec() > _timeLimit)
            {
                break;
            }

            const Unit & unit(state.getUnit(player, unitIndex));

            // iterate over each script move that it can execute
            for (size_t sIndex(0); sIndex<_playerScriptPortfolio.size(); ++sIndex)
            {
                // set the current script for this unit
                currentScriptData.setUnitScript(unit, _playerScriptPortfolio[sIndex]);

                // evaluate the current state given a playout with these unit scripts
                StateEvalScore score = eval(player, state, currentScriptData);

                // if we have a better score, set it
                if (sIndex == 0 || score > bestScoreVec[unitIndex])
                {
                    bestScriptVec[unitIndex] = _playerScriptPortfolio[sIndex];
                    bestScoreVec[unitIndex]  = score;

                    //std::cout << "Eval: " << score.val() << std::endl;
                }

                if((counterIterations == 0 && sIndex == 0) || score > bestScore)
                {
                	bestScore = score;
                    hasImproved = true;
                }
            }

            // set the current vector to the best move for use in future simulations
            currentScriptData.setUnitScript(unit, bestScriptVec[unitIndex]);
        }

    	//std::cout << "Completed Iteration without Improvements, iteration: " << counterIterations << " time elapsed: " << t.getElapsedTimeInMilliSec() << " time limit: " << _timeLimit << std::endl;


        if(!hasImproved)
        {
        	//std::cout << "Completed Iteration without Improvements, iteration: " << counterIterations << " time elapsed: " << t.getElapsedTimeInMilliSec() << std::endl;
            //std::cout << "Iteration: " << counterIterations << " LTD2 PGS: " << bestScore.val() << std::endl;
        	//break;
        	return;
        }

        counterIterations++;
    }   
}

IDType PortfolioGreedySearchNoTimeCache::calculateInitialSeed(const IDType & player, const GameState & state)
{
    IDType bestScript;
    StateEvalScore bestScriptScore;
    const IDType enemyPlayer(state.getEnemy(player));
    
    // try each script in the portfolio for each unit as an initial seed
    for (size_t sIndex(0); sIndex<_playerScriptPortfolio.size(); ++sIndex)
    {
        UnitScriptData currentScriptData;
    
        // set the player's chosen script initially to the seed choice
        for (size_t unitIndex(0); unitIndex < state.numUnits(player); ++unitIndex)
        {
            currentScriptData.setUnitScript(state.getUnit(player, unitIndex), _playerScriptPortfolio[sIndex]);
        }

        // set the enemy units script choice to NOKDPS
        for (size_t unitIndex(0); unitIndex < state.numUnits(enemyPlayer); ++unitIndex)
        {
            currentScriptData.setUnitScript(state.getUnit(enemyPlayer, unitIndex), _enemyScript);
        }

        // evaluate the current state given a playout with these unit scripts
        StateEvalScore score = eval(player, state, currentScriptData);

        if (sIndex == 0 || score > bestScriptScore)
        {
            bestScriptScore = score;
            bestScript = _playerScriptPortfolio[sIndex];
        }
    }

    return bestScript;
}

StateEvalScore PortfolioGreedySearchNoTimeCache::eval(const IDType & player, const GameState & state, UnitScriptData & playerScriptsChosen)
{
    const IDType enemyPlayer(state.getEnemy(player));
    
    if (_player == player) {
        ScoreType valCache = cacheLTD2->hitItemCache(playerScriptsChosen, player);
        StateEvalScore tempStateEval;

        if (valCache != -9999) {
            tempStateEval = StateEvalScore(valCache, 0);
            //std::cout << "Cache hit" << std::endl;
        } else {
            //std::cout << "Cache miss" << std::endl;
            

            Game g(state, 100);

            _totalEvals++;

            //return g.playLimitedIndividualScripts(player, playerScriptsChosen, 4);
            g.playIndividualScripts(playerScriptsChosen);
            tempStateEval = g.getState().eval(player, SparCraft::EvaluationMethods::LTD2);
            if (_qtdPlayoutIgnorar >= 2) {
                cacheLTD2->addItemCache(playerScriptsChosen, player, tempStateEval.val());
            } else {
                _qtdPlayoutIgnorar++;
            }

        }
        return tempStateEval;
    } else {       
            Game g(state, 100);

            _totalEvals++;
        //return g.playLimitedIndividualScripts(player, playerScriptsChosen, 4);
        g.playIndividualScripts(playerScriptsChosen);
        return g.getState().eval(player, SparCraft::EvaluationMethods::LTD2);
    }
}

void  PortfolioGreedySearchNoTimeCache::setAllScripts(const IDType & player, const GameState & state, UnitScriptData & data, const IDType & script)
{
    for (size_t unitIndex(0); unitIndex < state.numUnits(player); ++unitIndex)
    {
        data.setUnitScript(state.getUnit(player, unitIndex), script);
    }
}

/*
std::vector<Action> PortfolioGreedySparCraft::search(const IDType & player, const GameState & state)
{
    const IDType enemyPlayer(state.getEnemy(player));
	GameState initialState(state);
	MoveArray moves;
	state.generateMoves(moves, player);

    IDType seedScript = calculateInitialSeed(player, state);

    UnitScriptData currentScriptData;
    
    // set the player's chosen script initially to the seed choice
    for (size_t unitIndex(0); unitIndex < state.numUnits(player); ++unitIndex)
    {
        currentScriptData.setUnitScript(state.getUnit(player, unitIndex), seedScript);
    }

    // set the enemy units script choice to the seed as well
    for (size_t unitIndex(0); unitIndex < state.numUnits(enemyPlayer); ++unitIndex)
    {
        currentScriptData.setUnitScript(state.getUnit(enemyPlayer, unitIndex), _enemyScript);
    }
	
    std::vector<IDType> bestScriptVec(moves.numUnits(), seedScript);
	std::vector<StateEvalScore> bestScoreVec(moves.numUnits());

    // the current script vector we will be working with
    std::vector<IDType> currentScriptVec(moves.numUnits(), seedScript);

    // for each unit that can move
    for (size_t unitIndex(0); unitIndex<moves.numUnits(); ++unitIndex)
    {
        const Unit & unit(state.getUnit(player, unitIndex));

        // iterate over each script move that it can execute
        for (size_t sIndex(0); sIndex<_playerScriptPortfolio.size(); ++sIndex)
        {
            // set the current script for this unit
            currentScriptData.setUnitScript(unit, _playerScriptPortfolio[sIndex]);

            // evaluate the current state given a playout with these unit scripts
            StateEvalScore score = evalAfterMoves(player, state, currentScriptData);

            // if we have a better score, set it
            if (sIndex == 0 || score > bestScoreVec[unitIndex])
            {
                bestScriptVec[unitIndex] = _playerScriptPortfolio[sIndex];
                bestScoreVec[unitIndex]  = score;
            }
        }

        // set the current vector to the best move for use in future simulations
        currentScriptData.setUnitScript(unit, bestScriptVec[unitIndex]);
    }
	
    std::vector<Action> moveVec;
    currentScriptData.calculateMoves(player, moves, GameState(state), moveVec);
    return moveVec;
}*/