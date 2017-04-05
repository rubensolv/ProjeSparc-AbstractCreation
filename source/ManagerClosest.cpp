#include "ManagerClosest.h"

using namespace SparCraft;

ManagerClosest::ManagerClosest(const IDType & playerID, int numUnits) {
    this->_playerID = playerID;
    this->numUnits = numUnits;
}


ManagerClosest::~ManagerClosest() {
}

void ManagerClosest::controlUnitsForAB(GameState& state, const MoveArray& moves, std::set<Unit>& unidades) {
//verifico se as unidades não foram mortas
    std::set<Unit> tempUnitAbsAB;
    for (auto & un : unidades) {
        if (state.unitExist(_playerID, un.ID())) {
            tempUnitAbsAB.insert(un);
        }
    }
    unidades = tempUnitAbsAB;

    if (state.numUnits(_playerID) <= numUnits) {
        unidades.clear();
        //adiciono todas as unidades para serem controladas pelo AB
        for (int u(0); u < state.numUnits(_playerID); ++u) {
            unidades.insert(state.getUnit(_playerID, u));
        }
    } else if (!(unidades.size() == numUnits)) {

        if ((state.numUnits(_playerID) < 2 or moves.numUnits() < 2)
                and unidades.size() == 0) {
            unidades.insert(state.getUnit(_playerID, 0));
        } else {
            int control = 0;
            while (unidades.size() < numUnits and control < 20) {
                unidades.insert(state.getUnit(_playerID, rand() % state.numUnits(_playerID)));
                control++;
            }
        }
    }
}
