#ifndef CALCULATIONPRODUCER_H
#define CALCULATIONPRODUCER_H

#include "saprelement.h"

#include <iostream>
#include <vector>

class CalculationProducer
{
public:
    CalculationProducer();

    static void calculateArguments(std::vector<SaprElement*>);
    static std::vector<std::vector<std::vector<double>>> calcResults(SaprElement *firstElement);

    static std::unique_ptr<double[]> calcPoint(SaprElement *el, int i, double x);

    static void dropCalculation();

private:
    static std::vector<std::vector<double>> matrixA;
    static std::vector<double> matrixB;
    static std::vector<double> matrixDelt;
    static int size;

    static void prepareData(std::vector<SaprElement*>);
    static void calcDelt();
};

#endif // CALCULATIONPRODUCER_H
