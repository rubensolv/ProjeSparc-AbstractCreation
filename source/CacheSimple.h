#pragma once

#include "Common.h"
#include "CacheKeySimple.h"

namespace SparCraft {

    class CacheSimple {
    protected:
        std::unordered_map<CacheKeySimple, ScoreType> cache;
    public:
        CacheSimple();
        CacheSimple(const CacheSimple& orig);
        virtual ~CacheSimple();
        ScoreType hitItemCache(UnitScriptData & playerScript, const IDType & player);
        void addItemCache(UnitScriptData & playerScript, const IDType & player, ScoreType LTD2Value);
        
    private:
        

    };

}



