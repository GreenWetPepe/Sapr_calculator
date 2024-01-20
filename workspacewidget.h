#ifndef WORKSPACEWIDGET_H
#define WORKSPACEWIDGET_H

#include "projectwidget.h"
#include "SaprElementData.h"

#include <QWidget>
#include <QResizeEvent>

#include <vector>

namespace Ui {
class WorkSpaceWidget;
}

class WorkSpaceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WorkSpaceWidget(QWidget *parent = nullptr);
    ~WorkSpaceWidget();

    void addProjectWindow();

    void openProject();
    void saveProject();

signals:
    void changeMainWindowCentailWidgetToMenu();

private slots:
    void on_lengthLE_editingFinished();

    void on_squareLE_editingFinished();

    void on_elasticLE_editingFinished();

    void on_stressLE_editingFinished();

    void on_xLeftLE_editingFinished();

    void on_xRightLE_editingFinished();

    void on_qxLE_editingFinished();

    void on_leftSupCB_stateChanged(int arg1);

    void on_rightSupCB_stateChanged(int arg1);

    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::WorkSpaceWidget *ui;

    void markTabAsSaveState(ProjectWidget *projectWidget, bool isSaved);

    void resizeEvent(QResizeEvent *event);
    void setElementParameters(std::vector<SaprElement*> selectedElements);
    void clearElementParameters();
    void sendElementParametersToProjectWidget(std::vector<int> dataMask);

    std::vector<ProjectWidget*> projectWidgets;
};

#endif // WORKSPACEWIDGET_H
