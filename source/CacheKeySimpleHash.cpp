//#include "CacheKeySimpleHash.h"
#include "CacheSimple.h"

using namespace SparCraft;

CacheKeySimpleHash::CacheKeySimpleHash() {
}

CacheKeySimpleHash::CacheKeySimpleHash(const CacheKeySimpleHash& orig) {
}

CacheKeySimpleHash::~CacheKeySimpleHash() {
}

size_t CacheKeySimpleHash::operator()(const CacheKeySimple& cacheKey) const {   
    size_t retorno;
    for(auto & keys : cacheKey.scriptVector){
        retorno += std::hash<unsigned char>()(keys);
    }
    
    return retorno;
}

