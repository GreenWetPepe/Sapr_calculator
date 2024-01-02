#include "pointcalculator.h"
#include "ui_pointcalculator.h"

PointCalculator::PointCalculator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointCalculator)
{
    ui->setupUi(this);
}

PointCalculator::~PointCalculator()
{
    delete ui;
}

void PointCalculator::on_lineEdit_editingFinished()
{

}

