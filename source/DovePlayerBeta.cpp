

#include "DovePlayerBeta.h"

using namespace SparCraft;

DovePlayerBeta::DovePlayerBeta(const IDType& playerID) {
    _playerID = playerID;
    
}

void DovePlayerBeta::getMoves(GameState& state, const MoveArray& moves, std::vector<Action>& moveVec) {
    
    pgs = new PortfolioGreedySearchCache(_playerID, PlayerModels::NOKDPS, 1, 0, 35);
    
    Timer t;
    t.start();
    
    moveVec.clear();
    UnitScriptData PGSScriptData, POEScriptData;

    StateEvalScore PGSScore, POEScore;

    PGSScriptData = pgs->searchForScripts(_playerID, state, PGSScore);
    double ms = t.getElapsedTimeInMilliSec();
        
    if((40 - ms ) < 5 ){
        poe = new PortfolioOnlineEvolutionCache(_playerID, PlayerModels::NOKDPS, 1, 0, 5);
    }else{
        poe = new PortfolioOnlineEvolutionCache(_playerID, PlayerModels::NOKDPS, 1, 0,(40 - ms) );
    }
    POEScriptData = poe->searchForScripts(_playerID, state, POEScore);
    
    //analise POE
    Game g2(state, 100);
    g2.playIndividualScripts(POEScriptData);
    POEScore = g2.getState().eval(_playerID, SparCraft::EvaluationMethods::LTD2);
    
    MoveArray nMoves;
    state.generateMoves(nMoves, _playerID);
    GameState copy(state);
    
    if (POEScore > PGSScore) {    
        POEScriptData.calculateMoves(_playerID, nMoves, copy, moveVec);
    } else {
        PGSScriptData.calculateMoves(_playerID, nMoves, copy, moveVec);
    }    
    
    
    //ms = t.getElapsedTimeInMilliSec();
    //printf("\nMove PGS/POE chosen in %lf ms\n", ms);
    
    free(pgs);
    free(poe);
}

DovePlayerBeta::~DovePlayerBeta() {
}

