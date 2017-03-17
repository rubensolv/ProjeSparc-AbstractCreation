#pragma once

#include "Common.h"
#include <stdlib.h>
#include <string.h>
#include "Unit.h"

namespace SparCraft {

    class MetricGAB {
        int round;
        int numberUnits;
        int numberUnitsEnemy;
        ScoreType LTD2;
        std::vector<Unit> unitControlled;
        double timeExecution;
        std::string typeAlgoritm;
        double averageDistance;
        int numberAbstract;
    public:
        MetricGAB();
        MetricGAB(int round, int numberUnits) :
            round(round), numberUnits(numberUnits) {
        }
        MetricGAB(const MetricGAB& orig);
        virtual ~MetricGAB();        
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

    private:

    };
}



