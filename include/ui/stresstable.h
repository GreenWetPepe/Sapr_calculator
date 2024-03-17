#ifndef STRESSTABLE_H
#define STRESSTABLE_H

#include "workspace.h"

#include <QWidget>

namespace Ui {
class StressTable;
}

class StressTable : public QWidget
{
    Q_OBJECT

public:
    explicit StressTable(QWidget *parent = nullptr);
    ~StressTable();

    void initTable(WorkSpace workSpace);

private:
    Ui::StressTable *ui;

    void addRow(int i, double x, double permStress, double stress);
};

#endif // STRESSTABLE_H
