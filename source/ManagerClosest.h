#pragma once

#include "ManagerAbstraction.h"
#include <stdlib.h>


namespace SparCraft {

    class ManagerRandom : public ManagerAbstraction{
    public:
        ManagerRandom(const IDType & playerID, int numUnits);
        virtual ~ManagerRandom();
        void controlUnitsForAB(GameState & state, const MoveArray & moves, std::set<Unit> & unidades);
    private:

    };

}


