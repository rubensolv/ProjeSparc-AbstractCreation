#pragma once

#include "Common.h"
//#include "CacheKeySimple.h"
//#include "CacheKeySimpleHash.h"
#include <unordered_map>
#include "UnitScriptData.h"
#include "PortfolioOnlineGenome.h"


namespace SparCraft {

    class CacheSimpleString {
    protected:
        std::unordered_map<std::string, ScoreType> cache;
    public:
        CacheSimpleString();
        CacheSimpleString(const CacheSimpleString& orig);
        virtual ~CacheSimpleString();
        //PGS functions
        ScoreType hitItemCache(UnitScriptData & playerScript, const IDType & player);
        void addItemCache(UnitScriptData & playerScript, const IDType & player, ScoreType LTD2Value);
        void print();
        std::string readScriptData(UnitScriptData & currentScript, const IDType& player);
        //POE Functions
        std::string readPOGenome(PortfolioOnlineGenome & currentGenome, const IDType& player);
        ScoreType hitItemPOCache(PortfolioOnlineGenome & currentGenome, const IDType & player);
        void addPOItemCache(PortfolioOnlineGenome & currentGenome, const IDType & player, ScoreType LTD2Value);
    private:

    };



}



