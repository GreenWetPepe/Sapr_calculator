#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "menuwidget.h"
#include "workspacewidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MenuWidget *menuWidget = new MenuWidget();
    WorkSpaceWidget *workSpaceWidwet = new WorkSpaceWidget();

    MainWindow::setCentralWidget(workSpaceWidwet);
}

MainWindow::~MainWindow()
{
    delete ui;
}
