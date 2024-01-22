#include "stresstable.h"
#include "ui_stresstable.h"

StressTable::StressTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StressTable)
{
    ui->setupUi(this);
}

StressTable::~StressTable()
{
    delete ui;
}

void StressTable::initTable(WorkSpace workSpace)
{
    if (!workSpace.checkSystemReadiness()) close();

    CalculationProducer::dropCalculation();
    CalculationProducer::calculateArguments(workSpace.elements);
    std::vector<std::vector<std::vector<double>>> calculation = CalculationProducer::calcResults(workSpace.getFirstLinkedElement());

    SaprElement *el = workSpace.getFirstLinkedElement();
    int i = 0;
    while (el)
    {
        int j;
        for (j = 0; j < workSpace.elements.size(); j++)
        {
            if (el == workSpace.elements[j]) break;
        }

        addRow(j, 0, el->getPermissibleStress(), calculation[i][2][0]);
        addRow(j, el->getLength(), el->getPermissibleStress(), calculation[i][2].back());
        el = el->getRightConnectedElement();
        i++;
    }
}

void StressTable::addRow(int i, double x, double permStress, double stress)
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(i), 0));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(x), 0));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(permStress, 'f', 3), 0));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(stress, 'f', 3), 0));
    if (stress > permStress)
    {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++)
        {
            ui->tableWidget->item(ui->tableWidget->rowCount() - 1, j)->setBackgroundColor(Qt::red);
        }
    }
}
