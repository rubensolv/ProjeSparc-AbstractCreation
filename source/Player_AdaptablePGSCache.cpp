#include "Player_AdaptablePGSCache.h"

using namespace SparCraft;

Player_AdaptablePGSCache::Player_AdaptablePGSCache(const IDType & playerID) {
    _playerID = playerID;
    _iterations = 1;
    _responses = 0;
    _seed = PlayerModels::NOKDPS;
    _lastTimeState = 0;
    _numSteps = 100;
}

Player_AdaptablePGSCache::Player_AdaptablePGSCache(const IDType & playerID, const IDType & seed, const size_t & iter, const size_t & responses, const size_t & timeLimit) {
    _playerID = playerID;
    _iterations = iter;
    _responses = responses;
    _seed = seed;
    _timeLimit = timeLimit;
    _lastTimeState = 0;
    _numSteps = 100;
}

void Player_AdaptablePGSCache::getMoves(GameState & state, const MoveArray & moves, std::vector<Action> & moveVec) {
    moveVec.clear();
    
    if (_lastTimeState > state.getTime()) {
        _numSteps = 100;
    }
    _lastTimeState = state.getTime();

    AdaptablePGSPlusCache pgs(_playerID, _seed, _iterations, _responses, _timeLimit);
    pgs.setQtStepdsPlayout(_numSteps);
    StateEvalScore bestScore;
    moveVec = pgs.search(_playerID, state, bestScore);
    updateInfoPGS(pgs, state);
}

void Player_AdaptablePGSCache::updateInfoPGS(AdaptablePGSPlusCache & pgs, GameState & state) {
    //calcular o tempo do passo no playout
    double timeForStep = pgs.getTimePlayout()/_numSteps;
    
    _numSteps = 40/(pgs.getQtdScripts()*state.numUnits(_playerID)* pgs.getCounterIterations() * timeForStep);
    if(_numSteps > 1000){
        _numSteps = 1000;
    }
    /*
    std::cout <<"----> Dados Adaptable PGS ----"<<std::endl;
    std::cout <<"----> Quantidade de iterações para convergir = "<< pgs.getCounterIterations() << std::endl;
    std::cout << "----> Tempo por passo = "<< timeForStep << std::endl;
    std::cout <<"----> Num steps selecionados para a próxima rodada = "<< _numSteps << std::endl;
    */
}

