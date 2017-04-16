

#include "DovePlayer.h"

using namespace SparCraft;

DovePlayer::DovePlayer(const IDType& playerID) {
    _playerID = playerID;
    _costTimePlayout = 100;

}

void DovePlayer::getMoves(GameState& state, const MoveArray& moves, std::vector<Action>& moveVec) {
    if (canApplyExaustSearch(state)) {
        //aplico a busca exaustiva
        Timer t;
        t.start();
        double tempo = t.getElapsedTimeInMilliSec();
        executeExaustSearch(state, moves, moveVec);
    } else {
        executeDovetailing(state, moves, moveVec);
    }

}

void DovePlayer::executeExaustSearch(GameState& state, const MoveArray& moves, std::vector<Action>& moveVec) {
    std::vector<UnitScriptData> scripts = obtemSeqUnitScriptData(state, state.numUnits(_playerID), 2);

    UnitScriptData bestScript;
    StateEvalScore bestLTD2(ScoreType(-999999), 0);
    StateEvalScore tempEval;
    for (auto & unitScript : scripts) {
        tempEval = playUnitScripts(state, unitScript);
        if (tempEval > bestLTD2) {
            bestLTD2 = tempEval;
            bestScript = unitScript;
        }
    }
    // convert the script vector into a move vector and return it
    MoveArray movesT;
    state.generateMoves(movesT, _playerID);
    GameState copy(state);
    bestScript.calculateMoves(_playerID, movesT, copy, moveVec);
}

StateEvalScore DovePlayer::playUnitScripts(GameState& state, UnitScriptData script) {
    Game g2(state, 100);
    g2.playIndividualScripts(script);
    return g2.getState().eval(_playerID, SparCraft::EvaluationMethods::LTD2);
}

std::vector<UnitScriptData> DovePlayer::obtemSeqUnitScriptData(GameState& state, int n, int m) {
    int seq[n];
    std::vector<UnitScriptData> scripts;
    int i;
    for (i = 0; i < n; i++) seq[i] = 0;
    do {
        /*imprime sequência atual*/
        UnitScriptData temp;

        for (i = 0; i < n; i++) {
            if (seq[i] == 0) {
                temp.setUnitScript(state.getUnit(_playerID, i), PlayerModels::KiterDPS);
            } else {
                temp.setUnitScript(state.getUnit(_playerID, i), PlayerModels::NOKDPS);
            }
        }
        //gero os scripts para o inimigo
        IDType enemy(state.getEnemy(_playerID));
        for (size_t unitIndex(0); unitIndex < state.numUnits(enemy); ++unitIndex) {
            temp.setUnitScript(state.getUnit(enemy, unitIndex), PlayerModels::NOKDPS);
        }
        /*gera a próxima*/
        scripts.push_back(temp);
    } while (proxima(seq, n, m) == 0);

    return scripts;
}

int DovePlayer::proxima(int a[], int N, int M) {
    int t = N - 1;
    /*soma 1 ao vetor */
    while (t >= 0) {
        a[t] = (a[t] + 1) % M;
        if (a[t] == 0) t--;
        else return 0;
    }
    return -1;
}

DovePlayer::~DovePlayer() {
}

bool DovePlayer::canApplyExaustSearch(GameState& state) {
    double qtdUnits = (double) state.numUnits(_playerID);

    double qtdPlayouts = pow(2.0, qtdUnits);

    if ((qtdPlayouts * _costTimePlayout) <= 40) {
        return true;
    } else {
        return false;
    }

}

void DovePlayer::executeDovetailing(GameState& state, const MoveArray& moves, std::vector<Action>& moveVec) {
    pgs = new PortfolioGreedySearchCache(_playerID, PlayerModels::NOKDPS, 1, 0, 35);

    Timer t;
    t.start();

    moveVec.clear();
    UnitScriptData PGSScriptData, POEScriptData;

    StateEvalScore PGSScore, POEScore;

    PGSScriptData = pgs->searchForScripts(_playerID, state, PGSScore);
    double ms = t.getElapsedTimeInMilliSec();

    if ((40 - ms) < 5) {
        poe = new PortfolioOnlineEvolutionCache(_playerID, PlayerModels::NOKDPS, 1, 0, 5);
    } else {
        poe = new PortfolioOnlineEvolutionCache(_playerID, PlayerModels::NOKDPS, 1, 0, (40 - ms));
    }
    POEScriptData = poe->searchForScripts(_playerID, state, POEScore);

    //analise POE
    Game g2(state, 100);
    ms = t.getElapsedTimeInMilliSec();
    g2.playIndividualScripts(POEScriptData);
    POEScore = g2.getState().eval(_playerID, SparCraft::EvaluationMethods::LTD2);
    _costTimePlayout = t.getElapsedTimeInMilliSec() - ms;

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




