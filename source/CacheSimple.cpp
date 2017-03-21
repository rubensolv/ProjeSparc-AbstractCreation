

#include "CacheSimple.h"

using namespace SparCraft;

CacheSimple::CacheSimple() {
}

CacheSimple::CacheSimple(const CacheSimple& orig) {
}

CacheSimple::~CacheSimple() {
}

void CacheSimple::addItemCache(UnitScriptData& playerScript, const IDType& player, ScoreType LTD2Value) {
    //converter UnitScriptData em CacheKeySimple
    CacheKeySimple keyValue;
    keyValue.readScriptData(playerScript, player);
    //busca o valor no cache
    //if(cache.find(keyValue) == cache.end()){
        //não foi encontrado
        //cache[keyValue] = LTD2Value;
        cache.emplace(keyValue, LTD2Value);
        
    //}
    //????????????????????????????????????????????
    //atualizar o cache se existir a Key e o valor (LTD2) for maior?????????
    //????????????????????????????????????????????
}

ScoreType CacheSimple::hitItemCache(UnitScriptData& playerScript, const IDType& player) {
    ScoreType retorno(-9999);
    CacheKeySimple keyValue;
    keyValue.readScriptData(playerScript, player);
    
    if( !(cache.find(keyValue)==cache.end()) ){
        return cache.find(keyValue)->second;
    }
    
    return retorno;
}




