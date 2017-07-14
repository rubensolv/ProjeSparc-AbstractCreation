#pragma once

#include "Common.h"

#include "UnitScriptData.h"

namespace SparCraft {

    class UnifiedSearch {
        
    public:
        IDType _playerID;
        
        UnifiedSearch(IDType playerID, GameState & state);
        virtual ~UnifiedSearch();
        
        std::vector<Action> doSearchWithMoves(GameState & state, UnitScriptData & UnitScriptData, std::set<IDType> & unitsAB, size_t timeLimit);
    private:
        
    protected:
        StateEvalScore eval(std::vector<Action> moveVec, GameState& state);

    };
}

