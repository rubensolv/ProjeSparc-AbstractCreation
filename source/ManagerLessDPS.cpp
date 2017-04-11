#include "ManagerLessDPS.h"

using namespace SparCraft;

ManagerLessDPS::ManagerLessDPS(const IDType & playerID, int numUnits) {
    this->_playerID = playerID;
    this->numUnits = numUnits;
    srand(time(NULL));
}

ManagerLessDPS::~ManagerLessDPS() {
}

void ManagerLessDPS::controlUnitsForAB(GameState& state, const MoveArray& moves, std::set<Unit>& unidades) {

    //unitsDie(state, unidades);
    unidades.clear();

    if (unidades.size() == 0) {
        Unit & untBase(state.getUnitByID(_playerID, getIDUnitMoreDPS(state)));
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

IDType ManagerLessDPS::getIDUnitMoreDPS(GameState& state) {
    Unit un;
    double DPS(100000);
    Unit t;
    
    for (IDType u(0); u < state.numUnits(_playerID); ++u) {
        t = state.getUnit(_playerID, u);
        
        double DPSunit = ((double)t.damage()/(double)t.attackCooldown()) / t.currentHP();
        
        if(DPS > DPSunit){
            DPS = DPSunit;
            un = t;
        }
    }
    
    return un.ID();
}

bool ManagerLessDPS::hasMoreDPS(Unit& u1, Unit& u2) {
    
    double u1Threat = ((double)u1.damage()/(double)u1.attackCooldown()) / u1.currentHP();
    double u2Threat = ((double)u2.damage()/(double)u2.attackCooldown()) / u2.currentHP();

    return u1Threat > u2Threat;
}



bool ManagerLessDPS::existUnitsToAdd(std::set<Unit>& unidades, GameState& state) {
    Unit t;
    for (IDType u(0); u < state.numUnits(_playerID); ++u) {
        t = state.getUnit(_playerID, u);
        if(!unitExistInArray(t, unidades)){
            return true;
        }
    }
    
    return false;
}


IDType ManagerLessDPS::getIDUnitAdd(GameState& state, std::set<Unit>& unidades) {
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

bool ManagerLessDPS::unitExistInArray(Unit& unit, std::set<Unit>& unidades) {
    for(auto & un : unidades){
        if(un.equalsID(unit)){
            return true;
        }
    }
    return false;
}


void ManagerLessDPS::sortUnits(std::vector<Unit>& unidades, GameState & state) {

    for (int i = 1; i < unidades.size(); i++) {
        Unit key = unidades[i];
        int j = i - 1;
        while ((j >= 0)             
                && ( hasMoreDPS(unidades[j], key)  )
                ) {
            unidades[j + 1] = unidades[j];
            j--;
        }
        unidades[j + 1] = key;
    }
    
    /*
    std::cout << " INICIO -------------Unidades ordenadas-----------" << std::endl;
    double DPSunit = 0;
    for (auto & un : unidades) {
        un.print();
        DPSunit = ((double)un.damage()/(double)un.attackCooldown()) / un.currentHP();
        std::cout << DPSunit << std::endl;
    }

    std::cout << " FIM -------------Unidades ordenadas-----------" << std::endl;
    */
    
}

const PositionType ManagerLessDPS::getDistEuclidiana(const Position& pInicial, const Position& pFinal) {
    return sqrt(((pInicial.x() - pFinal.x())*(pInicial.x() - pFinal.x()) +
            (pInicial.y() - pFinal.y())*(pInicial.y() - pFinal.y())
            ));
}


void ManagerLessDPS::calcularCentroide(std::set<Unit>& unidades) {
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


void ManagerLessDPS::unitsDie(GameState& state, std::set<Unit>& unidades) {
    //verifico se as unidades não foram mortas
    std::set<Unit> tempUnitAbsAB;
    for (auto & un : unidades) {
        if (state.unitExist(_playerID, un.ID())) {
            tempUnitAbsAB.insert(state.getUnitByID(_playerID, un.ID()));
        }
    }
    unidades = tempUnitAbsAB;
}

void ManagerLessDPS::printUnits(std::set<Unit>& unidades) {
    std::cout << " INICIO -------------Relacão de unidades controladas-----------" << std::endl;
    for (auto & un : unidades) {
        un.print();
    }

    std::cout << " FIM -------------Relacão de unidades controladas-----------" << std::endl;
}
