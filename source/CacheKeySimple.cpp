
#include <vector>

//#include "CacheKeySimple.h"
#include "CacheSimple.h"

using namespace SparCraft;

CacheKeySimple::CacheKeySimple() {
}

CacheKeySimple::CacheKeySimple(const CacheKeySimple& orig) {
    scriptVector = orig.scriptVector;
}

CacheKeySimple::~CacheKeySimple() {
}

void CacheKeySimple::clear() {
    scriptVector.clear();
}

const bool CacheKeySimple::operator==(const CacheKeySimple& cacheKey) const {
    if(scriptVector.size() != cacheKey.scriptVector.size()){
        return false;
    }
    
    int qtdEl = scriptVector.size();
    
    for (int i = 0; i < qtdEl; i++) {
        if(scriptVector[i] != cacheKey.scriptVector[i]){
            return false;
        }
    }    
    
    return true;
}

void CacheKeySimple::readScriptData(UnitScriptData & currentScript, const IDType& player) {
    clear();
    for(auto & script : currentScript.getMapUnitScript(player)){
        scriptVector.push_back(script.second);
    }

}



