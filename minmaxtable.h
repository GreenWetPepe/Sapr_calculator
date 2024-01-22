#ifndef MINMAXTABLE_H
#define MINMAXTABLE_H

#include "workspace.h"
#include "calculationproducer.h"

#include <QWidget>

namespace Ui {
class MinMaxTable;
}

class MinMaxTable : public QWidget
{
    Q_OBJECT

public:
    explicit MinMaxTable(QWidget *parent = nullptr);
    ~MinMaxTable();

    void initTable(WorkSpace workSpace);

private:
    Ui::MinMaxTable *ui;

    void addRow(int i, double x, double nX, double uX, double sX);
};

#endif // MINMAXTABLE_H
