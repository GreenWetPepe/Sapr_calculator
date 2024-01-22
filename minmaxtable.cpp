#include "minmaxtable.h"
#include "ui_minmaxtable.h"

#include <vector>
#include <QDebug>

MinMaxTable::MinMaxTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MinMaxTable)
{
    ui->setupUi(this);
}

MinMaxTable::~MinMaxTable()
{
    delete ui;
}

void MinMaxTable::initTable(WorkSpace workSpace)
{
    if (!workSpace.checkSystemReadiness()) close();

    CalculationProducer::dropCalculation();
    CalculationProducer::calculateArguments(workSpace.elements);
    std::vector<std::vector<std::vector<double>>> calculation = CalculationProducer::calcResults(workSpace.getFirstLinkedElement());

    SaprElement *el = workSpace.getFirstLinkedElement();
    int i = 0;
    while (el)
    {
        int minUxId = 0, maxUxId = 0;
        double minUx = calculation[i][1][0], maxUx = calculation[i][1][0];
        int j;
        for (j = 0; j < workSpace.elements.size(); j++)
        {
            if (el == workSpace.elements[j]) break;
        }

        for (int k = 1; k < calculation[i][1].size() - 1; k++)
        {
            if (minUx > calculation[i][1][k])
            {
                minUx = calculation[i][1][k];
                minUxId = k;
            }
            if (maxUx < calculation[i][1][k])
            {
                maxUx = calculation[i][1][k];
                maxUxId = k;
            }
        }

        addRow(j, 0, calculation[i][0][0], calculation[i][1][0], calculation[i][2][0]);
        addRow(j, el->getLength(), calculation[i][0].back(), calculation[i][1].back(), calculation[i][2].back());
        if (minUx < calculation[i][1][0] && minUx < calculation[i][1].back())
        {
            addRow(j, (double(minUxId) * el->getLength()) / (calculation[i][1].size() - 1),
                   calculation[i][0][minUxId], calculation[i][1][minUxId], calculation[i][2][minUxId]);
        }
        if (maxUx > calculation[i][1][0] && maxUx > calculation[i][1].back())
        {
            addRow(j, (double(maxUxId) * el->getLength()) / (calculation[i][1].size() - 1),
                   calculation[i][0][maxUxId], calculation[i][1][maxUxId], calculation[i][2][maxUxId]);
        }
        el = el->getRightConnectedElement();
        i++;
    }
}

void MinMaxTable::addRow(int i, double x, double nX, double uX, double sX)
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(i), 0));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(x), 0));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(nX, 'f', 3), 0));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(uX, 'f', 3), 0));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, new QTableWidgetItem(QString::number(sX, 'f', 3), 0));
}
