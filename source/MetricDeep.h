#pragma once

#include "Common.h"
#include <stdlib.h>
#include <string.h>
#include "Unit.h"
#include "Timer.h"

namespace SparCraft {

    class MetricDeep {
        int round;
        int numberUnits;
        int numberUnitsEnemy;
        ScoreType LTD2;
        std::vector<Unit> unitControlled;
        double timeExecution;
        std::string typeAlgoritm;
        double averageDistance;
        int numberAbstract;
        std::string stateString;
    public:
        MetricDeep();
        MetricDeep(int round, int numberUnits) :
            round(round), numberUnits(numberUnits) {
        }
        MetricDeep(const MetricDeep& orig);
        virtual ~MetricDeep();   
        
        std::string getTimeToString();
        
        ScoreType GetLTD2() const {
            return LTD2;
        }

        void SetLTD2(ScoreType LTD2) {
            this->LTD2 = LTD2;
        }

        double GetAverageDistance() const {
            return averageDistance;
        }

        void SetAverageDistance(double averageDistance) {
            this->averageDistance = averageDistance;
        }

        int GetNumberAbstract() const {
            return numberAbstract;
        }

        void SetNumberAbstract(int numberAbstract) {
            this->numberAbstract = numberAbstract;
        }

        int GetNumberUnits() const {
            return numberUnits;
        }

        void SetNumberUnits(int numberUnits) {
            this->numberUnits = numberUnits;
        }

        int GetRound() const {
            return round;
        }

        void SetRound(int round) {
            this->round = round;
        }

        double GetTimeExecution() const {
            return timeExecution;
        }

        void SetTimeExecution(double timeExecution) {
            this->timeExecution = timeExecution;
        }

        std::string GetTypeAlgoritm() const {
            return typeAlgoritm;
        }

        void SetTypeAlgoritm(std::string typeAlgoritm) {
            this->typeAlgoritm = typeAlgoritm;
        }

        std::vector<Unit> GetUnitControlled() const {
            return unitControlled;
        }

        void SetUnitControlled(std::vector<Unit> unitControlled) {
            this->unitControlled = unitControlled;
        }
        
        void addUnitControled(Unit unit);
        
        void print();

        int GetNumberUnitsEnemy() const {
            return numberUnitsEnemy;
        }

        void SetNumberUnitsEnemy(int numberUnitsEnemy) {
            this->numberUnitsEnemy = numberUnitsEnemy;
        }
        
        std::string GetStateString() const {
            return stateString;
        }

        void SetStateString(std::string stateString) {
            this->stateString = stateString;
        }

    private:

    };
}



