/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MetricGAB.cpp
 * Author: rubens
 * 
 * Created on 6 de Março de 2017, 13:57
 */

#include "MetricDeep.h"

using namespace SparCraft;

MetricDeep::MetricDeep() {
    round = -1;
    numberUnits = -1;
    numberUnitsEnemy = -1;
    LTD2 = 0;
    timeExecution = 0.0;
    typeAlgoritm = "-";
    averageDistance = 0.0;
    numberAbstract = 0;
}


MetricDeep::MetricDeep(const MetricDeep& orig) {
}

MetricDeep::~MetricDeep() {
}

void MetricDeep::addUnitControled(Unit unit){
    unitControlled.push_back(unit);
}

void MetricDeep::print() {
    std::cout<< "---------------------------------------------------------------------- " <<std::endl;
    std::cout<< " Round = " << round<< std::endl;
    std::cout<< " Número de Unidades = " << numberUnits<< std::endl;
    std::cout<< " Número de Unidades Inimigas = " << numberUnitsEnemy<< std::endl;
    std::cout<< " LTD2 = " << LTD2 << std::endl;
    std::cout<< " Tempo de Execução = " << timeExecution<< std::endl;
    std::cout<< " Tipo de Algoritmo Utilizado = " << typeAlgoritm << std::endl;
    std::cout<< " Distancia Media das Unidades na Abstração = " << averageDistance << std::endl;
    std::cout<< " Número da Abstração = " << numberAbstract << std::endl;
    std::cout<< " Listagem de Unidades Controladas:" <<std::endl;
    for(auto & un : unitControlled){
        un.print();
    }
    std::cout<< "---------------------------------------------------------------------- " <<std::endl;
}

std::string MetricDeep::getTimeToString() {
    const time_t timer = time(NULL);
    struct tm *sTempo;
    sTempo = localtime(&timer);
    
    return std::to_string(sTempo->tm_mday)+"_"+ std::to_string(sTempo->tm_hour);
}




