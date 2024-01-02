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
    static std::vector<std::vector<double*>> calcResults(SaprElement*, int);

    static double* calcPoint(SaprElement *el, int i, double x);

    static void dropCalculation();

    static bool isReady();

private:
    static double** matrixA;
    static double* matrixB;
    static double* matrixDelt;
    static int size;

    static void prepareData(std::vector<SaprElement*>);
    static void calcDelt();
};

#endif // CALCULATIONPRODUCER_H
