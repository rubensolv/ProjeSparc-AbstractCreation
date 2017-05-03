#pragma once

#include "Common.h"
#include "GameState.h"
#include "Player.h"
#include "Game.h"
#include "Action.h"
#include "UnitScriptData.h"
#include "CacheSimpleString.h"
#include <memory>

namespace SparCraft {    
    typedef std::shared_ptr<Player> PlayerPtr;
    
    class CacheSimpleString;
    
    class AdaptablePGSPlusDaveCache {
        CacheSimpleString * cacheLTD2;
    protected:
        const IDType _player;
        const IDType _enemyScript;
        const size_t _iterations;
        const size_t _responses;
        std::vector<IDType> _playerScriptPortfolio;
        size_t _totalEvals;
        size_t _timeLimit;
        std::ofstream _fileTime;
        int _qtdPlayoutIgnorar;
        //for adaptable playout steps
        int _counterIterations;
        int _qtStepdsPlayout;
        double _timePlayout;

        void doPortfolioSearch(const IDType & player, const GameState & state, UnitScriptData & currentData, Timer & t, StateEvalScore & bestScore);
        std::vector<Action> getMoveVec(const IDType & player, const GameState & state, const std::vector<IDType> & playerScripts);
        StateEvalScore eval(const IDType & player, const GameState & state, UnitScriptData & playerScriptsChosen);
        IDType calculateInitialSeed(const IDType & player, const GameState & state);
        void setAllScripts(const IDType & player, const GameState & state, UnitScriptData & data, const IDType & script);

    public:

        AdaptablePGSPlusDaveCache(const IDType & player, const IDType & enemyScript, const size_t & iter, const size_t & responses, const size_t & timeLimit);
        ~AdaptablePGSPlusDaveCache();
        std::vector<Action> search(const IDType & player, const GameState & state, StateEvalScore & bestScore);
        UnitScriptData searchForScripts(const IDType & player, const GameState & state, StateEvalScore & bestScore);
        
        CacheSimpleString* getCacheLTD2() const {
            return cacheLTD2;
        }
        
        int getCounterIterations() const {
            return _counterIterations;
        }
        
        void setQtStepdsPlayout(int _qtStepdsPlayout) {
            this->_qtStepdsPlayout = _qtStepdsPlayout;
        }
        
        double getTimePlayout() const {
            return _timePlayout;
        }
        
        int getQtdScripts() const{
            return _playerScriptPortfolio.size();
        }
        
    };

}
