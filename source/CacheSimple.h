#pragma once

#include "Common.h"
//#include "CacheKeySimple.h"
//#include "CacheKeySimpleHash.h"
#include <unordered_map>
#include "UnitScriptData.h"


namespace SparCraft { 
    
    class CacheKeySimple {
        
    protected:

    public:
        std::vector<IDType> scriptVector;
        CacheKeySimple();
        CacheKeySimple(const CacheKeySimple& orig);
        virtual ~CacheKeySimple();
        void clear();
        const bool operator==(const CacheKeySimple &cacheKey) const;
        void readScriptData(UnitScriptData & currentScript, const IDType& player);
        void print();
    private:

    };
    
    class CacheKeySimpleHash {
    public:
        CacheKeySimpleHash();
        CacheKeySimpleHash(const CacheKeySimpleHash& orig);
        virtual ~CacheKeySimpleHash();
        size_t operator()(const CacheKeySimple & cacheKey) const;
    private:

    };
   
    class CacheSimple {
    protected:
        std::unordered_map<CacheKeySimple, ScoreType, CacheKeySimpleHash> cache;
    public:
        CacheSimple();
        CacheSimple(const CacheSimple& orig);
        virtual ~CacheSimple();
        ScoreType hitItemCache(UnitScriptData & playerScript, const IDType & player);
        void addItemCache(UnitScriptData & playerScript, const IDType & player, ScoreType LTD2Value);
        void print();
    private:
        

    };
    
    

}



