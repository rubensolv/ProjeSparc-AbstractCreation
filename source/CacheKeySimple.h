#pragma once

#include "Common.h"
#include "UnitScriptData.h"

namespace SparCraft {

    class CacheKeySimple {
        
    protected:
        std::vector<IDType> scriptVector;
    public:
        CacheKeySimple();
        CacheKeySimple(const CacheKeySimple& orig);
        virtual ~CacheKeySimple();
        void clear();
        const bool operator==(const CacheKeySimple &cacheKey) const;
        void readScriptData(UnitScriptData & currentScript, const IDType& player);
        
    private:

    };
}

