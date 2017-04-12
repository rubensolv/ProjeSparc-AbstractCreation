

#include "DovePlayerAlpha.h"

using namespace SparCraft;

DovePlayerAlpha::DovePlayerAlpha(const IDType& playerID) {
    _playerID = playerID;
    pgs = new PortfolioGreedySearchCache(_playerID, PlayerModels::NOKDPS, 1, 0, 40);
    poe = new PortfolioOnlineEvolutionCache(_playerID, PlayerModels::NOKDPS, 1, 0, 40);
}

void DovePlayerAlpha::getMoves(GameState& state, const MoveArray& moves, std::vector<Action>& moveVec) {
    Timer t;
    t.start();
    moveVec.clear();
    UnitScriptData PGSScriptData, POEcriptData;

    StateEvalScore PGSScore, POEScore;

    PGSScriptData = pgs->searchForScripts(_playerID, state, PGSScore);
    POEcriptData = poe->searchForScripts(_playerID, state, POEScore);

    MoveArray nMoves;
    state.generateMoves(nMoves, _playerID);
    GameState copy(state);
    std::cout << "PGSScore = " << PGSScore.val() << " POEScore =" << POEScore.val()<< std::endl;
    
    if (POEScore > PGSScore) {    
        POEcriptData.calculateMoves(_playerID, nMoves, copy, moveVec);
    } else {
        PGSScriptData.calculateMoves(_playerID, nMoves, copy, moveVec);
    }
    
    PGSScriptData.calculateMoves(_playerID, nMoves, copy, moveVec);
    
    double ms = t.getElapsedTimeInMilliSec();
    printf("\nMove PGS chosen in %lf ms\n", ms);
    
}

DovePlayerAlpha::~DovePlayerAlpha() {
    free(pgs);
    free(poe);
}

