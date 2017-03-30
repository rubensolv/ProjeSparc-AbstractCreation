#pragma once

#include "Common.h"
//#include "CacheKeySimple.h"
//#include "CacheKeySimpleHash.h"
#include <unordered_map>
#include "UnitScriptData.h"


namespace SparCraft {

    class CacheSimpleString {
    protected:
        std::unordered_map<std::string, ScoreType> cache;
    public:
        CacheSimpleString();
        CacheSimpleString(const CacheSimpleString& orig);
        virtual ~CacheSimpleString();
        ScoreType hitItemCache(UnitScriptData & playerScript, const IDType & player);
        void addItemCache(UnitScriptData & playerScript, const IDType & player, ScoreType LTD2Value);
        void print();
        std::string readScriptData(UnitScriptData & currentScript, const IDType& player);
    private:

    };



}



