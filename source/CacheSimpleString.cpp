

#include "CacheSimpleString.h"

using namespace SparCraft;

CacheSimpleString::CacheSimpleString() {
}

CacheSimpleString::CacheSimpleString(const CacheSimpleString& orig) {
}

CacheSimpleString::~CacheSimpleString() {
}

void CacheSimpleString::addItemCache(UnitScriptData& playerScript, const IDType& player, ScoreType LTD2Value) {
    //converter UnitScriptData em CacheKeySimple
    //CacheKeySimple keyValue;
    //keyValue.readScriptData(playerScript, player);
    //keyValue.print();
    //busca o valor no cache
    //if(cache.find(keyValue) == cache.end()){
        //não foi encontrado
        //cache[keyValue] = LTD2Value;
        cache.emplace(readScriptData(playerScript,player), LTD2Value);
        
    //}
    //????????????????????????????????????????????
    //atualizar o cache se existir a Key e o valor (LTD2) for maior?????????
    //????????????????????????????????????????????
}

ScoreType CacheSimpleString::hitItemCache(UnitScriptData& playerScript, const IDType& player) {
    ScoreType retorno(-9999);
    //CacheKeySimple keyValue;
    //keyValue.readScriptData(playerScript, player);
    std::string buscarKey = readScriptData(playerScript, player);
    if( !(cache.find(buscarKey)==cache.end()) ){
        return cache.find(buscarKey)->second;
    }
    
    return retorno;
}

void CacheSimpleString::print() {
    std::cout<< "Tamanho do cache: " << cache.size() <<std::endl;
}


std::string CacheSimpleString::readScriptData(UnitScriptData & currentScript, const IDType& player) {
    std::string retorno = "";
    for(auto & script : currentScript.getMapUnitScript(player)){
        retorno += std::to_string(script.second) + ";";
    }
    return retorno;
}

void CacheSimpleString::addPOItemCache(PortfolioOnlineGenome& currentGenome, const IDType& player, ScoreType LTD2Value) {
    cache.emplace(readPOGenome(currentGenome,player), LTD2Value);
}


ScoreType CacheSimpleString::hitItemPOCache(PortfolioOnlineGenome& currentGenome, const IDType& player) {
    ScoreType retorno(-9999);
    //CacheKeySimple keyValue;
    //keyValue.readScriptData(playerScript, player);
    std::string buscarKey = readPOGenome(currentGenome, player);
    
    //std::cout << "Chave procurada = "<< buscarKey << std::endl;
    
    if( !(cache.find(buscarKey)==cache.end()) ){
        return cache.find(buscarKey)->second;
    }
    
    return retorno;
}



std::string CacheSimpleString::readPOGenome(PortfolioOnlineGenome& currentGenome, const IDType& player) {
    std::string retorno = "";
    for(auto & script : currentGenome.getMapUnitScript(player)){
        retorno += std::to_string(script.second) + ";";
    }
    return retorno;
}

