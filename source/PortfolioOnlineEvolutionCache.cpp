#include "PortfolioOnlineEvolutionCache.h"

using namespace SparCraft;

PortfolioOnlineEvolutionCache::PortfolioOnlineEvolutionCache(const IDType & player, const IDType & enemyScript, const size_t & iter, const size_t & responses, const size_t & timeLimit)
	: _player(player)
	, _enemyScript(enemyScript)
	, _iterations(iter)
    , _responses(responses)
    , _totalEvals(0)
    , _timeLimit(timeLimit)
	, _populationSize(16)
	, _playoutLimit(25)
	, _selectedMembers(4)
	, _offspringPerSelected(3)
{
	_playerScriptPortfolio.push_back(PlayerModels::NOKDPS);
	_playerScriptPortfolio.push_back(PlayerModels::KiterDPS);
	//_playerScriptPortfolio.push_back(PlayerModels::Cluster);
//	_playerScriptPortfolio.push_back(PlayerModels::MoveForward);
//	_playerScriptPortfolio.push_back(PlayerModels::MoveBackward);

	srand(1234);
        cacheLTD2 = new CacheSimpleString();
        _qtdPlayoutIgnorar = 0;
}

void PortfolioOnlineEvolutionCache::init(const IDType & player, const GameState & state, std::vector<PortfolioOnlineGenome> & population)
{
	for(int i = 0; i < this->_populationSize; i++)
	{
		PortfolioOnlineGenome gen(player, state);
		population.push_back(gen);
	}
}

void PortfolioOnlineEvolutionCache::mutatePopulation(const IDType & player, const GameState & state, std::vector<PortfolioOnlineGenome> & population)
{
	std::vector<PortfolioOnlineGenome>  newPopulation;
	//std::cout << "Population size before mutation: " << population.size() << " selected: " << _selectedMembers << std::endl;
	for(int j = 0; j < population.size(); j++)
	{
		//population[j].mutate(player, state, _playerScriptPortfolio);
		for(int i = 0; i < _offspringPerSelected; i++)
		{
			PortfolioOnlineGenome offspring(player, state, population[j], _playerScriptPortfolio);
			newPopulation.push_back(offspring);
		}
	}
	for(int i = 0; i < newPopulation.size(); i++)
	{
		population.push_back(newPopulation[i]);
	}
//	std::cout << "Population size after mutation: " << population.size() << std::endl << std::endl;
}

void PortfolioOnlineEvolutionCache::select(const IDType & player, const GameState & state, std::vector<PortfolioOnlineGenome> & population)
{
	std::vector<PortfolioOnlineGenome> newPopulation;
	for(int i = 0; i < _selectedMembers; i++)
	{
		newPopulation.push_back(population[i]);
	}
	population.clear();
	population = newPopulation;
}

void PortfolioOnlineEvolutionCache::crossover(const IDType & player, const GameState & state, std::vector<PortfolioOnlineGenome> & population)
{
	std::vector<PortfolioOnlineGenome> newPopulation;
	for(int i = 0; i < _selectedMembers; i++)
	{
		newPopulation.push_back(population[i]);
	}

	for(int i = 0; i < _selectedMembers; i++)
	{
		for(int j = 0; j < _selectedMembers; j++)
		{
			if(i == j)
			{
				continue;
			}

			PortfolioOnlineGenome offspring(player, state, population[rand() % population.size()], population[rand() % population.size()]);
			offspring.mutate(player, state, _playerScriptPortfolio);
			newPopulation.push_back(offspring);
		}
	}
	population.clear();
	population = newPopulation;
}

StateEvalScore PortfolioOnlineEvolutionCache::evalPopulation(const IDType & player, const GameState & state, std::vector<PortfolioOnlineGenome> & population)
{
    //std::cout << "Análise hit cache POE Cache"<< std::endl;
	const IDType enemyPlayer(state.getEnemy(player));
	for(int i = 0; i < population.size(); i++)
	{
            
            //se o jogador for igual ao nosso jogador
            if((player == _player) ){
                ScoreType valCache = cacheLTD2->hitItemPOCache(population[i], player);
                StateEvalScore tempStateEval;
                if(valCache != -9999){
                 //   std::cout << "Cache hit "<< valCache << std::endl;
                    population[i].setFitness(StateEvalScore(valCache, 0));
                }else{
                    Game g(state, 100);
                    _totalEvals++;
                    tempStateEval = g.playoutGenome(player, population[i], _playoutLimit);
                    population[i].setFitness(tempStateEval); 
                    //std::cout << "Cache miss "<< tempStateEval.val() << std::endl;
                    cacheLTD2->addPOItemCache(population[i], player, tempStateEval.val());
                }
                
                
            }else{
		Game g(state, 100);
		_totalEvals++;
		population[i].setFitness(g.playoutGenome(player, population[i], _playoutLimit));
            }
	}

	std::sort(population.begin(), population.end());
        
        //std::cout << " FIM Análise hit cache POE Cache"<< std::endl;
        return population[0].getFitness();
}

std::vector<Action> PortfolioOnlineEvolutionCache::search(const IDType & player, const GameState & state)
{
    Timer t;
    t.start();
    
    //std::cout << " Iniciando POE Cache " << std::endl;

    const IDType enemyPlayer(state.getEnemy(player));

    double ms = t.getElapsedTimeInMilliSec();
    //printf("\nFirst Part %lf ms\n", ms);

    std::vector<PortfolioOnlineGenome> population;
    init(player, state, population);
    
    ScoreType evalReturn = -9999;
    StateEvalScore scoreFitness(evalReturn, 0);
    int numberMutate = 0, maxChange = 0;

    while((ms < this->_timeLimit) and  (numberMutate < 4) )
    {
    	StateEvalScore tmpEval = evalPopulation(player, state, population);
    	select(player, state, population);
    	mutatePopulation(player, state, population);
    	//crossover(player, state, population);

    	ms = t.getElapsedTimeInMilliSec();
        if(scoreFitness < tmpEval  and maxChange < 2){
            scoreFitness = tmpEval;
            //std::cout<<"Score Fitness Parcial = "<< (double) tmpEval.val()<<std::endl;
            maxChange = 0;
        }else{
            //std::cout<<"Score Fitness Final = "<< (double) scoreFitness.val() <<std::endl;
            if(maxChange >=1){
                //std::cout<<"Parei o Fitness"<<std::endl;
                break;
            }else{
                //std::cout<<"Incrementei MaxChange"<<std::endl;
                maxChange++;
            }
            
        }
        numberMutate++;
    }

    //ms = t.getElapsedTimeInMilliSec();
    //printf("\nMove POE chosen in %lf ms\n", ms);

	evalPopulation(player, state, population);

    // convert the script vector into a move vector and return it
	MoveArray moves;
	state.generateMoves(moves, player);
    std::vector<Action> moveVec;
    GameState copy(state);
    population[0].calculateMoves(player, moves, copy, moveVec);

    _totalEvals = 0;

    //std::cout << " Encerrando POE Cache " << std::endl;
    return moveVec;
}


UnitScriptData PortfolioOnlineEvolutionCache::searchForScripts(const IDType& player, const GameState& state, StateEvalScore & bestScore){
    Timer t;
    t.start();

    const IDType enemyPlayer(state.getEnemy(player));

    double ms = t.getElapsedTimeInMilliSec();
    //printf("\nFirst Part %lf ms\n", ms);

    std::vector<PortfolioOnlineGenome> population;
    init(player, state, population);
    
    ScoreType evalReturn = -9999;
    StateEvalScore scoreFitness(evalReturn, 0);    
    
    while(ms < this->_timeLimit )
    {
    	StateEvalScore tmpEval = evalPopulation(player, state, population);
    	select(player, state, population);
    	mutatePopulation(player, state, population);
    	//crossover(player, state, population);
        
        
        ms = t.getElapsedTimeInMilliSec();
    
    }

//    ms = t.getElapsedTimeInMilliSec();
//    printf("\nMove POE chosen in %lf ms\n", ms);
    
    bestScore = evalPopulation(player, state, population);
    //population[0].
    PortfolioOnlineGenome pop = population[0];
    UnitScriptData currentScriptData;
    
    for (size_t unitIndex(0); unitIndex < state.numUnits(player); ++unitIndex)
    {
        currentScriptData.setUnitScript(state.getUnit(player, unitIndex), pop.getUnitScript(state.getUnit(player, unitIndex)));
    }
    
    for (size_t unitIndex(0); unitIndex < state.numUnits(enemyPlayer); ++unitIndex)
    {
        currentScriptData.setUnitScript(state.getUnit(enemyPlayer, unitIndex), pop.getUnitScript(state.getUnit(enemyPlayer, unitIndex)));
    }

    return currentScriptData;
}

PortfolioOnlineGenome PortfolioOnlineEvolutionCache::searchForGenome(const IDType& player, const GameState& state, StateEvalScore & bestScore) {
    Timer t;
    t.start();

    const IDType enemyPlayer(state.getEnemy(player));

    double ms = t.getElapsedTimeInMilliSec();
    //printf("\nFirst Part %lf ms\n", ms);

    std::vector<PortfolioOnlineGenome> population;
    init(player, state, population);
    
    ScoreType evalReturn = -9999;
    StateEvalScore scoreFitness(evalReturn, 0);
    int numberMutate = 0, maxChange = 0;

    while((ms < this->_timeLimit) and  (numberMutate < 4) )
    {
    	StateEvalScore tmpEval = evalPopulation(player, state, population);
    	select(player, state, population);
    	mutatePopulation(player, state, population);
    	//crossover(player, state, population);

    	ms = t.getElapsedTimeInMilliSec();
        if(scoreFitness < tmpEval  and maxChange < 2){
            scoreFitness = tmpEval;
            //std::cout<<"Score Fitness Parcial = "<< (double) tmpEval.val()<<std::endl;
            maxChange = 0;
        }else{
            //std::cout<<"Score Fitness Final = "<< (double) scoreFitness.val() <<std::endl;
            if(maxChange >=1){
                //std::cout<<"Parei o Fitness"<<std::endl;
                break;
            }else{
                //std::cout<<"Incrementei MaxChange"<<std::endl;
                maxChange++;
            }
            
        }
        numberMutate++;
    }

//    ms = t.getElapsedTimeInMilliSec();
//    printf("\nMove POE chosen in %lf ms\n", ms);
    
     bestScore = evalPopulation(player, state, population);    
    
    return population[0];
}
