
#include "CacheKeySimple.h"

using namespace SparCraft;

CacheKeySimple::CacheKeySimple() {
}

CacheKeySimple::CacheKeySimple(const CacheKeySimple& orig) {
    this->scriptVector = orig.scriptVector;
}

CacheKeySimple::~CacheKeySimple() {
}

void CacheKeySimple::clear() {
    scriptVector.clear();
}

const bool CacheKeySimple::operator==(const CacheKeySimple& cacheKey) const {
    if(this->scriptVector.size() != cacheKey.scriptVector.size()){
        return false;
    }
    
    int qtdEl = this->scriptVector.size();
    
    for (int i = 0; i < qtdEl; i++) {
        if(this->scriptVector[i] != cacheKey.scriptVector[i]){
            return false;
        }
    }    
    
    return true;
}

void CacheKeySimple::readScriptData(UnitScriptData& currentScript, const IDType& player) {
    clear();
    for(auto & script : currentScript.getMapUnitScript(player)){
        scriptVector.push_back(script.second);
    }

}



