#include "AdaptableStratifiedPolicySearchLimitSim.h"

using namespace SparCraft;

AdaptableStratifiedPolicySearchLimitSim::AdaptableStratifiedPolicySearchLimitSim(const IDType & player, const IDType & enemyScript, const size_t & iter, const size_t & responses, const size_t & timeLimit)
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
	_playerScriptPortfolio.push_back(PlayerModels::Kiter_NOKDPS);
	_playerScriptPortfolio.push_back(PlayerModels::MoveBackward);
        _playerScriptPortfolio.push_back(PlayerModels::AttackWeakest);
        _playerScriptPortfolio.push_back(PlayerModels::AttackClosest);
}

std::vector<Action> AdaptableStratifiedPolicySearchLimitSim::search(const IDType & player, const GameState & state, StateEvalScore & bestScore)
{
    Timer t;
    t.start();

    const IDType enemyPlayer(state.getEnemy(player));

    // set up the root script data
    UnitScriptData originalScriptData;
    setAllScripts(player, state, originalScriptData, PlayerModels::NOKDPS);
    setAllScripts(enemyPlayer, state, originalScriptData, PlayerModels::NOKDPS);

    // do the initial root portfolio search for our player
    UnitScriptData currentScriptData(originalScriptData);
    int numberTypes;
	double timePlayout;

    if(doStratifiedSearch(player, state, currentScriptData, t, numberTypes, timePlayout, bestScore))
    {
    	//std::cout << timePlayout << ", ";
        //AdaptableStratType::printType();
    	//std::cout << "Types: " << numberTypes << " Time: " << timePlayout << std::endl;
    	AdaptableStratType::increase(timePlayout, _timeLimit, _playerScriptPortfolio.size());
    }
    else
    {
    	//std::cout << timePlayout << ", ";
        //AdaptableStratType::printType();
    	//std::cout << "Types: " << numberTypes << " Time: " << timePlayout << std::endl;
    	AdaptableStratType::decrease(numberTypes);
    }

    //double ms = t.getElapsedTimeInMilliSec();
    //printf("\nMove SPSLimit chosen in %lf ms\n", ms);
    // convert the script vector into a move vector and return it
	MoveArray moves;
	state.generateMoves(moves, player);
    std::vector<Action> moveVec;
    GameState copy(state);
    currentScriptData.calculateMoves(player, moves, copy, moveVec);

    _totalEvals = 0;

    return moveVec;
}

UnitScriptData AdaptableStratifiedPolicySearchLimitSim::searchForScripts(const IDType& player, const GameState& state, StateEvalScore& bestScore) {
    Timer t;
    t.start();

    const IDType enemyPlayer(state.getEnemy(player));

    // set up the root script data
    UnitScriptData originalScriptData;
    setAllScripts(player, state, originalScriptData, PlayerModels::NOKDPS);
    setAllScripts(enemyPlayer, state, originalScriptData, PlayerModels::NOKDPS);

    // do the initial root portfolio search for our player
    UnitScriptData currentScriptData(originalScriptData);
    int numberTypes;
	double timePlayout;

    if(doStratifiedSearch(player, state, currentScriptData, t, numberTypes, timePlayout, bestScore))
    {
    	//std::cout << timePlayout << ", ";
        //AdaptableStratType::printType();
    	//std::cout << "Types: " << numberTypes << " Time: " << timePlayout << std::endl;
    	AdaptableStratType::increase(timePlayout, _timeLimit, _playerScriptPortfolio.size());
    }
    else
    {
    	//std::cout << timePlayout << ", ";
        //AdaptableStratType::printType();
    	//std::cout << "Types: " << numberTypes << " Time: " << timePlayout << std::endl;
    	AdaptableStratType::decrease(numberTypes);
    }

    //double ms = t.getElapsedTimeInMilliSec();
    //printf("\nMove SPSLimit chosen in %lf ms\n", ms);
    // convert the script vector into a move vector and return it
    //MoveArray moves;
    //state.generateMoves(moves, player);
    //std::vector<Action> moveVec;
    //GameState copy(state);
    //currentScriptData.calculateMoves(player, moves, copy, moveVec);

    _totalEvals = 0;

    return currentScriptData;
}


bool AdaptableStratifiedPolicySearchLimitSim::doStratifiedSearch(const IDType & player, const GameState & state, UnitScriptData & currentScriptData, Timer & timer, int & numberTypes, double & timePlayouts, StateEvalScore & bestScore)
{
	int numberEvals = 0;

    // the enemy of this player
    const IDType enemyPlayer(state.getEnemy(player));

    //compute the set of type for each unit that can move
    std::map<AdaptableStratType, std::vector<size_t> > typeUnits;
    for (size_t unitIndex(0); unitIndex<state.numUnits(player); ++unitIndex)
    {
        const Unit & unit(state.getUnit(player, unitIndex));
        AdaptableStratType t(unit, state);
        if(typeUnits.find(t) == typeUnits.end())
        {
        	std::vector<size_t> v;
        	typeUnits[t] = v;
        }
        typeUnits[t].push_back(unitIndex);
    }
    
    //std::cout << "Number of types: " << typeUnits.size() << std::endl;

    //number of types to be returned through parameter numberTypes
    numberTypes = typeUnits.size();

    bool hasFinishedIteration = false;
    Timer t;
    t.start();
    int counterIterations = 0;
    //for (size_t i(0); i<_iterations; ++i)
    while(timer.getElapsedTimeInMilliSec() < _timeLimit)
    {
        // set up data for best scripts
    	IDType          bestScriptVec[typeUnits.size()];
    	StateEvalScore  bestScoreVec[typeUnits.size()];
        bool hasImproved = false;

    	std::map<AdaptableStratType, std::vector<size_t> >::iterator it = typeUnits.begin();
        for(int typeIndex = 0; typeIndex < typeUnits.size(); typeIndex++, ++it)
        {
            for (size_t sIndex(0); sIndex<_playerScriptPortfolio.size(); ++sIndex)
            {
            	for(int j = 0; j < (it->second).size(); j++)
            	{
                    currentScriptData.setUnitScript(state.getUnit(player, (it->second)[j]), _playerScriptPortfolio[sIndex]);
            	}

              //  Timer t;
              //  t.start();
                StateEvalScore score = eval(player, state, currentScriptData);
              //  std::cout << state.numUnits(player) << ": " << t.getElapsedTimeInMilliSec() << " ms." << std::endl;
                numberEvals++;

                // if we have a better score, set it
                if (sIndex == 0 || score > bestScoreVec[typeIndex])
                {
                    bestScriptVec[typeIndex] = _playerScriptPortfolio[sIndex];
                    bestScoreVec[typeIndex]  = score;
                }
                if((counterIterations == 0 && sIndex == 0) || score > bestScore)
                {
                    bestScore = score;
                    hasImproved = true;
                }
            }

        	for(int j = 0; j < (it->second).size(); j++)
        	{
                currentScriptData.setUnitScript(state.getUnit(player, (it->second)[j]), bestScriptVec[typeIndex]);
        	}

            if (_timeLimit > 0 && timer.getElapsedTimeInMilliSec() > _timeLimit)
            {
            	timePlayouts = t.getElapsedTimeInMilliSec() / ((double) numberEvals);
            	return hasFinishedIteration;
            }
        }
        
        if(!hasImproved)
        {
            //std::cout << "Completed Iteration without Improvements, iteration: " << counterIterations << " time elapsed: " << t.getElapsedTimeInMilliSec() << std::endl;
            //std::cout << "Iteration: " << counterIterations << " LTD2 PGS: " << bestScore.val() << std::endl;
        	//break;
        	return hasFinishedIteration;
        }
        counterIterations++;
        hasFinishedIteration = true;
    }

	timePlayouts = t.getElapsedTimeInMilliSec() / ((double) numberEvals);
    return hasFinishedIteration;
}

IDType AdaptableStratifiedPolicySearchLimitSim::calculateInitialSeed(const IDType & player, const GameState & state)
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

StateEvalScore AdaptableStratifiedPolicySearchLimitSim::eval(const IDType & player, const GameState & state, UnitScriptData & playerScriptsChosen)
{
    const IDType enemyPlayer(state.getEnemy(player));
    Game g(state, 100);
    _totalEvals++;
    return g.playLimitedIndividualScripts(player, playerScriptsChosen, 4);
}

void  AdaptableStratifiedPolicySearchLimitSim::setAllScripts(const IDType & player, const GameState & state, UnitScriptData & data, const IDType & script)
{
    for (size_t unitIndex(0); unitIndex < state.numUnits(player); ++unitIndex)
    {
        data.setUnitScript(state.getUnit(player, unitIndex), script);
    }
}
