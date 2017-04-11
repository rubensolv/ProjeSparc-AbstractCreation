#include "ManagerMoreLife.h"

using namespace SparCraft;

ManagerMoreLife::ManagerMoreLife(const IDType & playerID, int numUnits) {
    this->_playerID = playerID;
    this->numUnits = numUnits;
    srand(time(NULL));
}

ManagerMoreLife::~ManagerMoreLife() {
}

void ManagerMoreLife::controlUnitsForAB(GameState& state, const MoveArray& moves, std::set<Unit>& unidades) {

    //unitsDie(state, unidades);
    unidades.clear();

    if (unidades.size() == 0) {
        Unit & untBase(state.getUnitByID(_playerID, getIDUnitMoreLife(state)));
        unidades.insert(untBase);
    }    
    
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
            }
            //unidades.insert(state.getUnit(_playerID, rand() % state.numUnits(_playerID)));
            control++;
        }
    }
    
}

IDType ManagerMoreLife::getIDUnitMoreLife(GameState& state) {
    Unit un;
    HealthType lessHP(-9999);
    Unit t;
    for (IDType u(0); u < state.numUnits(_playerID); ++u) {
        t = state.getUnit(_playerID, u);
        if(lessHP < t.currentHP()){
            lessHP = t.currentHP();
            un = t;
        }
    }
    
    return un.ID();
}


bool ManagerMoreLife::existUnitsToAdd(std::set<Unit>& unidades, GameState& state) {
    Unit t;
    for (IDType u(0); u < state.numUnits(_playerID); ++u) {
        t = state.getUnit(_playerID, u);
        if(!unitExistInArray(t, unidades)){
            return true;
        }
    }
    
    return false;
}


IDType ManagerMoreLife::getIDUnitAdd(GameState& state, std::set<Unit>& unidades) {
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

bool ManagerMoreLife::unitExistInArray(Unit& unit, std::set<Unit>& unidades) {
    for(auto & un : unidades){
        if(un.equalsID(unit)){
            return true;
        }
    }
    return false;
}


void ManagerMoreLife::sortUnits(std::vector<Unit>& unidades, GameState & state) {
    for (int i = 1; i < unidades.size(); i++) {
        Unit key = unidades[i];
        int j = i - 1;
        while ((j >= 0)             
                && (unidades[j].currentHP() <= key.currentHP())
                ) {
            unidades[j + 1] = unidades[j];
            j--;
        }
        unidades[j + 1] = key;
    }
    /*
    std::cout << " INICIO -------------Unidades ordenadas-----------" << std::endl;
    for (auto & un : unidades) {
        un.print();
    }

    std::cout << " FIM -------------Unidades ordenadas-----------" << std::endl;
    */
    
}

const PositionType ManagerMoreLife::getDistEuclidiana(const Position& pInicial, const Position& pFinal) {
    return sqrt(((pInicial.x() - pFinal.x())*(pInicial.x() - pFinal.x()) +
            (pInicial.y() - pFinal.y())*(pInicial.y() - pFinal.y())
            ));
}


void ManagerMoreLife::calcularCentroide(std::set<Unit>& unidades) {
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


void ManagerMoreLife::unitsDie(GameState& state, std::set<Unit>& unidades) {
    //verifico se as unidades não foram mortas
    std::set<Unit> tempUnitAbsAB;
    for (auto & un : unidades) {
        if (state.unitExist(_playerID, un.ID())) {
            tempUnitAbsAB.insert(state.getUnitByID(_playerID, un.ID()));
        }
    }
    unidades = tempUnitAbsAB;
}

void ManagerMoreLife::printUnits(std::set<Unit>& unidades) {
    std::cout << " INICIO -------------Relacão de unidades controladas-----------" << std::endl;
    for (auto & un : unidades) {
        un.print();
    }

    std::cout << " FIM -------------Relacão de unidades controladas-----------" << std::endl;
}
