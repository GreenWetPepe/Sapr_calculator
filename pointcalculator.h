#ifndef POINTCALCULATOR_H
#define POINTCALCULATOR_H

#include <QWidget>

namespace Ui {
class PointCalculator;
}

class PointCalculator : public QWidget
{
    Q_OBJECT

public:
    explicit PointCalculator(QWidget *parent = nullptr);
    ~PointCalculator();

private slots:
    void on_lineEdit_editingFinished();

private:
    Ui::PointCalculator *ui;
};

#endif // POINTCALCULATOR_H
