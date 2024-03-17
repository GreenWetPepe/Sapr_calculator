#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "menuwidget.h"
#include "workspacewidget.h"

#include <QMainWindow>
#include <QStackedWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_createProjectAction_triggered();

    void on_openProjectAction_triggered();

    void on_saveProjectAction_triggered();

    void on_calcConstructAction_triggered();

    void on_createMixMaxTableAction_triggered();

    void on_createStressTableAction_triggered();

private:
    Ui::MainWindow *ui;

    MenuWidget *menuWidget = nullptr;
    WorkSpaceWidget *workSpaceWidget = nullptr;
    QStackedWidget stackedWidget;

    void changeWidgetToMenuWidget();
};

#endif // MAINWINDOW_H
