#include "PortfolioOnlineEvolutionDeep.h"

using namespace SparCraft;

PortfolioOnlineEvolutionDeep::PortfolioOnlineEvolutionDeep(const IDType & player, const IDType & enemyScript, const size_t & iter, const size_t & responses, const size_t & timeLimit, int numPartida)
: _player(player)
, _enemyScript(enemyScript)
, _iterations(iter)
, _responses(responses)
, _totalEvals(0)
, _timeLimit(timeLimit)
, _populationSize(16)
, _playoutLimit(25)
, _selectedMembers(4)
, _offspringPerSelected(3) {
    _playerScriptPortfolio.push_back(PlayerModels::NOKDPS);
    _playerScriptPortfolio.push_back(PlayerModels::KiterDPS);
    //_playerScriptPortfolio.push_back(PlayerModels::Cluster);
    //	_playerScriptPortfolio.push_back(PlayerModels::MoveForward);
    //	_playerScriptPortfolio.push_back(PlayerModels::MoveBackward);
    controlPartidas = numPartida;
    srand(1234);
}

void PortfolioOnlineEvolutionDeep::init(const IDType & player, const GameState & state, std::vector<PortfolioOnlineGenome> & population) {
    for (int i = 0; i < this->_populationSize; i++) {
        PortfolioOnlineGenome gen(player, state);
        population.push_back(gen);
    }
}

void PortfolioOnlineEvolutionDeep::mutatePopulation(const IDType & player, const GameState & state, std::vector<PortfolioOnlineGenome> & population) {
    std::vector<PortfolioOnlineGenome> newPopulation;
    //std::cout << "Population size before mutation: " << population.size() << " selected: " << _selectedMembers << std::endl;
    for (int j = 0; j < population.size(); j++) {
        //population[j].mutate(player, state, _playerScriptPortfolio);
        for (int i = 0; i < _offspringPerSelected; i++) {
            PortfolioOnlineGenome offspring(player, state, population[j], _playerScriptPortfolio);
            newPopulation.push_back(offspring);
        }
    }
    for (int i = 0; i < newPopulation.size(); i++) {
        population.push_back(newPopulation[i]);
    }
    //	std::cout << "Population size after mutation: " << population.size() << std::endl << std::endl;
}

void PortfolioOnlineEvolutionDeep::select(const IDType & player, const GameState & state, std::vector<PortfolioOnlineGenome> & population) {
    std::vector<PortfolioOnlineGenome> newPopulation;
    for (int i = 0; i < _selectedMembers; i++) {
        newPopulation.push_back(population[i]);
    }
    population.clear();
    population = newPopulation;
}

void PortfolioOnlineEvolutionDeep::crossover(const IDType & player, const GameState & state, std::vector<PortfolioOnlineGenome> & population) {
    std::vector<PortfolioOnlineGenome> newPopulation;
    for (int i = 0; i < _selectedMembers; i++) {
        newPopulation.push_back(population[i]);
    }

    for (int i = 0; i < _selectedMembers; i++) {
        for (int j = 0; j < _selectedMembers; j++) {
            if (i == j) {
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

void PortfolioOnlineEvolutionDeep::evalPopulation(const IDType & player, const GameState & state, std::vector<PortfolioOnlineGenome> & population) {
    const IDType enemyPlayer(state.getEnemy(player));
    for (int i = 0; i < population.size(); i++) {
        Game g(state, 100);
        _totalEvals++;
        population[i].setFitness(g.playoutGenome(player, population[i], _playoutLimit));
    }

    std::sort(population.begin(), population.end());
}

std::vector<Action> PortfolioOnlineEvolutionDeep::search(const IDType & player, const GameState & state) {
    Timer t;
    MetricDeep newMetric;
    t.start();

    newMetric.SetNumberUnits(state.numUnits(player));
    newMetric.SetNumberUnitsEnemy(state.numUnits(state.getEnemy(player)));
    newMetric.SetRound(state.getTime());

    newMetric.SetNumberAbstract(controlPartidas);
    newMetric.SetStateString(state.toString());
    newMetric.SetTypeAlgoritm("POENormal");

    const IDType enemyPlayer(state.getEnemy(player));

    double ms = t.getElapsedTimeInMilliSec();
    //printf("\nFirst Part %lf ms\n", ms);

    std::vector<PortfolioOnlineGenome> population;
    init(player, state, population);

    while (ms < this->_timeLimit) {
        evalPopulation(player, state, population);
        select(player, state, population);
        mutatePopulation(player, state, population);
        //crossover(player, state, population);

        ms = t.getElapsedTimeInMilliSec();
    }

    //    ms = t.getElapsedTimeInMilliSec();
    //    printf("\nMove POE chosen in %lf ms\n", ms);

    evalPopulation(player, state, population);

    // convert the script vector into a move vector and return it
    MoveArray moves;
    state.generateMoves(moves, player);
    std::vector<Action> moveVec;
    GameState copy(state);
    population[0].calculateMoves(player, moves, copy, moveVec);

    _totalEvals = 0;
    //Execução POE
    Game temp(state,100);
    temp.playoutGenome(player, population[0], 100);
    
    newMetric.SetLTD2(temp.playoutGenome(player, population[0], 100).val());
    newMetric.SetTimeExecution(t.getElapsedTimeInMilliSec());
    saveMetrics(newMetric, moveVec);

    return moveVec;
}

UnitScriptData PortfolioOnlineEvolutionDeep::searchForScripts(const IDType& player, const GameState& state) {
    Timer t;
    t.start();

    const IDType enemyPlayer(state.getEnemy(player));

    double ms = t.getElapsedTimeInMilliSec();
    //printf("\nFirst Part %lf ms\n", ms);

    std::vector<PortfolioOnlineGenome> population;
    init(player, state, population);

    while (ms < this->_timeLimit) {
        evalPopulation(player, state, population);
        select(player, state, population);
        mutatePopulation(player, state, population);
        //crossover(player, state, population);

        ms = t.getElapsedTimeInMilliSec();
    }

    //    ms = t.getElapsedTimeInMilliSec();
    //    printf("\nMove POE chosen in %lf ms\n", ms);

    evalPopulation(player, state, population);
    //population[0].
    PortfolioOnlineGenome pop = population[0];
    UnitScriptData currentScriptData;

    for (size_t unitIndex(0); unitIndex < state.numUnits(player); ++unitIndex) {
        currentScriptData.setUnitScript(state.getUnit(player, unitIndex), pop.getUnitScript(state.getUnit(player, unitIndex)));
    }

    for (size_t unitIndex(0); unitIndex < state.numUnits(enemyPlayer); ++unitIndex) {
        currentScriptData.setUnitScript(state.getUnit(enemyPlayer, unitIndex), pop.getUnitScript(state.getUnit(enemyPlayer, unitIndex)));
    }

    return currentScriptData;
}

void PortfolioOnlineEvolutionDeep::saveMetrics(MetricDeep& metrica, std::vector<Action>& moveVec) {
    mkdir("resultDeep", 0777);
    std::ofstream arquivo;

    std::string nomeArq = "resultDeep/PortfolioOnlineEvolutionDeep_" + std::to_string(_player) + "_" + metrica.getTimeToString() + "_" + std::to_string(metrica.GetNumberAbstract());

    arquivo.open(nomeArq, std::ios_base::app | std::ios_base::out);

    if (!arquivo.is_open()) {
        System::FatalError("Problem Opening Output File: Arquivo de metrica");
    }

    arquivo << " Round = " << metrica.GetRound() << std::endl;
    arquivo << " Número de Unidades = " << metrica.GetNumberUnits() << std::endl;
    arquivo << " Número de Unidades Inimigas = " << metrica.GetNumberUnitsEnemy() << std::endl;
    arquivo << " LTD2 = " << metrica.GetLTD2() << std::endl;
    arquivo << " Tempo de Execução = " << metrica.GetTimeExecution() << std::endl;
    arquivo << " Tipo de Algoritmo Utilizado = " << metrica.GetTypeAlgoritm() << std::endl;
    arquivo << " Distancia Media das Unidades na Abstração = " << metrica.GetAverageDistance() << std::endl;
    arquivo << " Número da Abstração = " << metrica.GetNumberAbstract() << std::endl;

    arquivo << " Cópia do estado:" << std::endl;
    arquivo << metrica.GetStateString() << std::endl;

    arquivo << " Movimentos sugeridos:" << std::endl;
    for (auto & act : moveVec) {
        arquivo << act.debugString() << std::endl;
    }

    arquivo << std::endl;
    arquivo.close();
}
