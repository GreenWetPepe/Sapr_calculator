#include "largestnumbersmodulo.h"
#include "ui_largestnumbersmodulo.h"

#include "calculationproducer.h"

largestNumbersModulo::largestNumbersModulo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::largestNumbersModulo)
{
    ui->setupUi(this);
    table = findChild<QTableWidget*>();
}

largestNumbersModulo::~largestNumbersModulo()
{
    delete ui;
}

void largestNumbersModulo::fillTable()
{
    CalculationProducer::calculateArguments(elements);

    SaprElement *firstElement = nullptr;
    for (auto el : elements)
    {
        if (!el->leftConnectedElement)
        {
            firstElement = el;
            break;
        }
    }
    std::vector<std::vector<double*>> calculation = CalculationProducer::calcResults(firstElement, pointsCount);

    table->blockSignals(true);
    table->setRowCount(0);
    int i = 0;
    while (firstElement)
    {
        fillCriticalValues(firstElement, i, i, calculation);
        firstElement = firstElement->rightConnectedElement;
        i++;
    }
//    for (int i = 0; i < elements.size(); i++)
//    {
//        SaprElement *e = firstElement;
//        int j = 0;
//        while (e)
//        {
//            if (e == elements[i]) break;
//            e = e->rightConnectedElement;
//            j++;
//        }

//        fillCriticalValues(elements[i], j, i, calculation);
//    }
    table->blockSignals(false);
}


void largestNumbersModulo::fillCriticalValues(SaprElement* element, int id, int vectorId, std::vector<std::vector<double*>> calculation)
{
    double* zeroRes = CalculationProducer::calcPoint(element, id, 0);
    fillResultTableRow(vectorId, zeroRes, 0);
    double* lastRes = CalculationProducer::calcPoint(element, id, element->length);
    fillResultTableRow(vectorId, lastRes, element->length);

    for (int i = 0; i < 3; i++)
    {
        double minNum = zeroRes[i], maxNum = zeroRes[i];
        int minX = 0, maxX = 0;
        for (int j = 0; j <= pointsCount; j++)
        {
            if (minNum > calculation[id][i][j])
            {
                minNum = calculation[id][i][j];
                minX = j;
            }

            if (maxNum < calculation[id][i][j])
            {
                maxNum = calculation[id][i][j];
                maxX = j;
            }
        }

        if (minX != 0 && minX != pointsCount)
        {
            double x = minX * element->length / (pointsCount);
            fillResultTableRow(vectorId, CalculationProducer::calcPoint(element, id, x), x);
        }
        if (maxX != 0 && maxX != pointsCount)
        {
            double x = maxX * element->length / (pointsCount);
            fillResultTableRow(vectorId, CalculationProducer::calcPoint(element, id, x), x);
        }
    }
}

void largestNumbersModulo::fillResultTableRow(int id, double* calc, double x)
{
    table->insertRow(table->rowCount());
    setItem(table, table->rowCount() - 1, 0, QString::number(id));
    setItem(table, table->rowCount() - 1, 1, QString::number(x));
    setItem(table, table->rowCount() - 1, 2, QString::number(calc[0]));
    setItem(table, table->rowCount() - 1, 3, QString::number(calc[1]));
    setItem(table, table->rowCount() - 1, 4, QString::number(calc[2]));
}

void largestNumbersModulo::setItem(QTableWidget *table, int row, int column, QString value)
{
    QTableWidgetItem *item = table->item(row, column);
    if (!item)
    {
        item = new QTableWidgetItem(0);
        table->setItem(row, column, item);
    }
    item->setText(value);
}
