#pragma once

#include "ManagerAbstraction.h"
#include <stdlib.h>


namespace SparCraft {

    class ManagerClosest : public ManagerAbstraction{
    public:
        ManagerClosest(const IDType & playerID, int numUnits);
        virtual ~ManagerClosest();
        void controlUnitsForAB(GameState & state, const MoveArray & moves, std::set<Unit> & unidades);
    private:

    };

}


