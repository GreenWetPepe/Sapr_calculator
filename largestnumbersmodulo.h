#ifndef LARGESTNUMBERSMODULO_H
#define LARGESTNUMBERSMODULO_H

#include "saprelement.h"

#include <QWidget>
#include <QTableWidget>
#include <vector>

namespace Ui {
class largestNumbersModulo;
}

class largestNumbersModulo : public QWidget
{
    Q_OBJECT

public:
    explicit largestNumbersModulo(QWidget *parent = nullptr);
    ~largestNumbersModulo();

    std::vector<SaprElement*> elements;
    void fillTable();

private:
    Ui::largestNumbersModulo *ui;

    void fillCriticalValues(SaprElement* element, int id, int vectorId, std::vector<std::vector<double*>> calculation);
    void fillResultTableRow(int id, double* calc, double x);
    void setItem(QTableWidget *table, int row, int column, QString value);
    QTableWidget *table = nullptr;
    int pointsCount = 1000;
};

#endif // LARGESTNUMBERSMODULO_H
