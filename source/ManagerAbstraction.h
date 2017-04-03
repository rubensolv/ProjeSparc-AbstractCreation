#pragma once

#include "Common.h"
#include "GameState.h"
#include "Unit.h"
#include "MoveArray.h"

namespace SparCraft {
    
    class GameState;

    class ManagerAbstraction {
    public:
        IDType _playerID;
        int numUnits;
        ManagerAbstraction();
        ManagerAbstraction(const ManagerAbstraction& orig);
        virtual ~ManagerAbstraction();
        virtual void controlUnitsForAB(GameState & state, const MoveArray & moves, std::set<Unit> & unidades);
    private:

    };
}



