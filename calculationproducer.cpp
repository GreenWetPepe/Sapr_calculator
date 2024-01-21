#include "calculationproducer.h"
#include "Options.cpp"


#include <QDebug>
#include <fstream>

int CalculationProducer::size = 0;
std::vector<std::vector<double>> CalculationProducer::matrixA;
std::vector<double> CalculationProducer::matrixB;
std::vector<double> CalculationProducer::matrixDelt;

CalculationProducer::CalculationProducer()
{

}

std::unique_ptr<double[]> CalculationProducer::calcPoint(SaprElement *el, int i, double x)
{
    std::unique_ptr<double[]> res(new double[3]);

    res[0] = (el->getElasticModulus() * el->getSquare() / el->getLength()) * (matrixDelt[i + 1] - matrixDelt[i]) +
             (el->getXQForce() * el->getLength() / 2) * (1 - 2 * (x / el->getLength()));
    res[1] = matrixDelt[i] + (x / el->getLength()) * (matrixDelt[i + 1] - matrixDelt[i]) +
             el->getXQForce() * el->getLength() * el->getLength() / (2 * el->getElasticModulus() * el->getSquare()) *
                                                                    (x / el->getLength()) * (1 - (x / el->getLength()));
    res[2] = res[0] / el->getSquare();

    return res;
}

void CalculationProducer::calculateArguments(std::vector<SaprElement*> elements)
{
    prepareData(elements);
    calcDelt();
}

void CalculationProducer::prepareData(std::vector<SaprElement*> elements)
{
    size = elements.size() + 1;
    matrixA.clear();
    matrixB.clear();
    for (int i = 0; i < size; i++)
    {
        matrixA.push_back(std::vector<double>(size));
        matrixB.push_back(0);
        for (int j = 0; j < size; j++)
        {
            matrixA[i][j] = 0;
        }
    }
    SaprElement *firstElement = nullptr;
    for (auto el : elements)
    {
        if (!el->getLeftConnectedElement())
        {
            firstElement = el;
            break;
        }
    }

    if (!firstElement) return;

    SaprElement *element = firstElement;
    int c = 0;
    while(true)
    {
        double cellVar = element->getElasticModulus() * element->getSquare() / element->getLength();
        matrixA[c][c] += cellVar;
        matrixA[c + 1][c + 1] += cellVar;
        matrixA[c + 1][c] = -cellVar;
        matrixA[c][c + 1] = -cellVar;

        matrixB[c] += element->getXLeftForce() + element->getXQForce() * element->getLength() / 2;
        matrixB[c + 1] += element->getXQForce() * element->getLength() / 2;

        if (!element->getRightConnectedElement()) break;
        element = element->getRightConnectedElement();
        c++;
    }
    matrixB[c + 1] += element->getXRightForce();

    if (firstElement->hasLeftSupport())
    {
        matrixA[0][0] = 1;
        matrixA[0][1] = 0;
        matrixA[1][0] = 0;
        matrixB[0] = 0;
    }
    if (element->hasRightSupport())
    {
        matrixA[c + 1][c + 1] = 1;
        matrixA[c + 1][c] = 0;
        matrixA[c][c + 1] = 0;
        matrixB[c + 1] = 0;
    }

}

void CalculationProducer::calcDelt()
{
    double** tempA = new double*[size];
    double* tempB = new double[size];
    for (int i = 0; i < size; i++)
    {
        tempA[i] = new double[size];
        tempB[i] = matrixB[i];
        for (int j = 0; j < size; j++)
        {
            tempA[i][j] = matrixA[i][j];
        }
    }

    matrixDelt.clear();
    matrixDelt.resize(size);
    for (int i = 0; i < size - 1; i++)
    {
        if (matrixA[i + 1][i] != 0)
        {
            tempA[i + 1][i] = 0;
            tempA[i + 1][i + 1] -= tempA[i][i + 1] * (matrixA[i + 1][i] / tempA[i][i]);
            tempB[i + 1] -= tempB[i] * (matrixA[i + 1][i] / tempA[i][i]);
        }
    }
    for (int i = size - 1; i > 0; i--)
    {
        matrixDelt[i] = tempB[i] / tempA[i][i];
        if (tempA[i - 1][i] != 0)
        {
            tempB[i - 1] -= matrixDelt[i] * tempA[i - 1][i];
            tempA[i - 1][i] = 0;
        }
    }
    matrixDelt[0] = tempB[0] / tempA[0][0];

    for (int i = 0; i < size; i++)
    {
        delete[] tempA[i];
    }
    delete[] tempA;
    delete[] tempB;
}

std::vector<std::vector<std::vector<double>>> CalculationProducer::calcResults(SaprElement *firstElement)
{
    std::vector<std::vector<std::vector<double>>> res;
    if (matrixDelt.empty()) return res;
    SaprElement* el = firstElement;

    int i = 0;
    while (el != nullptr)
    {
        res.push_back(std::vector<std::vector<double>>());
        res.back().push_back(std::vector<double>());
        res.back().push_back(std::vector<double>());
        res.back().push_back(std::vector<double>());
        for (int j = 0; j < options::diagram::pointsCount; j++)
        {
            res.back()[0].push_back((el->getElasticModulus() * el->getSquare() / el->getLength()) * (matrixDelt[i + 1] - matrixDelt[i]) +
                        (el->getXQForce() * el->getLength() / 2) * (1 - 2 * (double(j) / (options::diagram::pointsCount - 1))));
            res.back()[1].push_back(matrixDelt[i] + (double(j) / (options::diagram::pointsCount - 1)) * (matrixDelt[i + 1] - matrixDelt[i]) +
                        el->getXQForce() * el->getLength() * el->getLength() / (2 * el->getElasticModulus() * el->getSquare()) *
                        (double(j) / (options::diagram::pointsCount - 1)) *
                        (1 - (double(j) / (options::diagram::pointsCount - 1))));
            res.back()[2].push_back(res.back()[0][j] / el->getSquare());
        }
        i++;
        el = el->getRightConnectedElement();
    }

    return res;
}

void CalculationProducer::dropCalculation()
{
    matrixA.clear();
    matrixB.clear();
    matrixDelt.clear();
}
