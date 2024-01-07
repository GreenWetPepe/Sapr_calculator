#ifndef WORKSPACEWIDGET_H
#define WORKSPACEWIDGET_H

#include <QWidget>

namespace Ui {
class WorkSpaceWidget;
}

class WorkSpaceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WorkSpaceWidget(QWidget *parent = nullptr);
    ~WorkSpaceWidget();

private:
    Ui::WorkSpaceWidget *ui;
};

#endif // WORKSPACEWIDGET_H
