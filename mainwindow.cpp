#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "workspacewidget.h"
#include "menuwidget.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MenuWidget *menuWidget = new MenuWidget();
    WorkSpaceWidget *workSpaceWidwet = new WorkSpaceWidget();

    setCentralWidget(workSpaceWidwet);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_createProjectAction_triggered()
{
    qDebug() << "UWU";
}


void MainWindow::on_openProjectAction_triggered()
{

}


void MainWindow::on_saveProjectAction_triggered()
{

}

