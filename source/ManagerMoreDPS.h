#pragma once

#include "ManagerAbstraction.h"
#include <stdlib.h>



namespace SparCraft {
    
    class ManagerMoreDPS : public ManagerAbstraction{
        Position _centroide;
    public:
        ManagerMoreDPS(const IDType & playerID, int numUnits);
        virtual ~ManagerMoreDPS();
        void controlUnitsForAB(GameState & state, const MoveArray & moves, std::set<Unit> & unidades);
    private:
        void unitsDie(GameState& state,std::set<Unit>& unidades);
        void printUnits(std::set<Unit>& unidades);
        void calcularCentroide(std::set<Unit>& unidades);
        IDType getIDUnitAdd(GameState& state,std::set<Unit>& unidades);
        void sortUnits(std::vector<Unit>& unidades, GameState & state);
        const PositionType getDistEuclidiana(const Position& pInicial, const Position& pFinal);
        bool unitExistInArray(Unit & unit, std::set<Unit>& unidades);
        bool existUnitsToAdd(std::set<Unit>& unidades, GameState & state);
        IDType getIDUnitMoreDPS(GameState & state);
        bool  hasMoreDPS(Unit & u1, Unit & u2);
    };

}


