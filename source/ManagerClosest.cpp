#include "ManagerClosest.h"

using namespace SparCraft;

ManagerClosest::ManagerClosest(const IDType & playerID, int numUnits) {
    this->_playerID = playerID;
    this->numUnits = numUnits;
    srand(time(NULL));
}

ManagerClosest::~ManagerClosest() {
}

void ManagerClosest::controlUnitsForAB(GameState& state, const MoveArray& moves, std::set<Unit>& unidades) {

    unitsDie(state, unidades);

    if (unidades.size() == 0) {
        Unit & untBase(state.getUnit(_playerID, (rand() % state.numUnits(_playerID))));
        unidades.insert(untBase);
    }
    
    calcularCentroide(unidades);
    
    if (state.numUnits(_playerID) <= numUnits) {
        unidades.clear();
        //adiciono todas as unidades para serem controladas pelo AB
        for (int u(0); u < state.numUnits(_playerID); ++u) {
            unidades.insert(state.getUnit(_playerID, u));
        }
    } else if (!(unidades.size() == numUnits)) {
        
        int control = 0;
        while (unidades.size() < numUnits and control < 20) {
            if(existUnitsToAdd(unidades, state)){
                unidades.insert(state.getUnitByID(_playerID, getIDUnitAdd(state, unidades)));
                calcularCentroide(unidades);
            }
            //unidades.insert(state.getUnit(_playerID, rand() % state.numUnits(_playerID)));
            control++;
        }

    }
}

bool ManagerClosest::existUnitsToAdd(std::set<Unit>& unidades, GameState& state) {
    Unit t;
    for (IDType u(0); u < state.numUnits(_playerID); ++u) {
        t = state.getUnit(_playerID, u);
        if(!unitExistInArray(t, unidades)){
            return true;
        }
    }
    
    return false;
}


IDType ManagerClosest::getIDUnitAdd(GameState& state, std::set<Unit>& unidades) {
    std::vector<Unit> unitOrdenar;
    Unit t;
    for (IDType u(0); u < state.numUnits(_playerID); ++u) {
        t = state.getUnit(_playerID, u);
        if(!unitExistInArray(t, unidades)){
            unitOrdenar.push_back(t);
        }
    }
    sortUnits(unitOrdenar, state);
    
    return unitOrdenar[0].ID();
    
}

bool ManagerClosest::unitExistInArray(Unit& unit, std::set<Unit>& unidades) {
    for(auto & un : unidades){
        if(un.equalsID(unit)){
            return true;
        }
    }
    return false;
}


void ManagerClosest::sortUnits(std::vector<Unit>& unidades, GameState & state) {
    for (int i = 1; i < unidades.size(); i++) {
        Unit key = unidades[i];
        int j = i - 1;
        while ((j >= 0) && (getDistEuclidiana(_centroide, unidades[j].currentPosition(state.getTime()))
                > getDistEuclidiana(_centroide, key.currentPosition(state.getTime())))
                && (unidades[j].currentHP() >= key.currentHP())
                ) {
            unidades[j + 1] = unidades[j];
            j--;
        }
        unidades[j + 1] = key;
    }
}

const PositionType ManagerClosest::getDistEuclidiana(const Position& pInicial, const Position& pFinal) {
    return sqrt(((pInicial.x() - pFinal.x())*(pInicial.x() - pFinal.x()) +
            (pInicial.y() - pFinal.y())*(pInicial.y() - pFinal.y())
            ));
}


void ManagerClosest::calcularCentroide(std::set<Unit>& unidades) {
    PositionType x(0), y(0);
    
    for(auto & un : unidades){
        x += un.position().x();
        y += un.position().y();
    }
    x = x / unidades.size();
    y = y / unidades.size();
    Position newCentroid(x,y);
    _centroide = newCentroid;
}


void ManagerClosest::unitsDie(GameState& state, std::set<Unit>& unidades) {
    //verifico se as unidades não foram mortas
    std::set<Unit> tempUnitAbsAB;
    for (auto & un : unidades) {
        if (state.unitExist(_playerID, un.ID())) {
            tempUnitAbsAB.insert(state.getUnitByID(_playerID, un.ID()));
        }
    }
    unidades = tempUnitAbsAB;
}

void ManagerClosest::printUnits(std::set<Unit>& unidades) {
    std::cout << " INICIO -------------Relacão de unidades controladas-----------" << std::endl;
    for (auto & un : unidades) {
        un.print();
    }

    std::cout << " FIM -------------Relacão de unidades controladas-----------" << std::endl;
}
