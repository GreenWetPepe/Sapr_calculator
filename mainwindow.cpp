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

    menuWidget = new MenuWidget();
    workSpaceWidget = new WorkSpaceWidget();

    connect(workSpaceWidget, &WorkSpaceWidget::changeMainWindowCentailWidgetToMenu, this, &MainWindow::changeWidgetToMenuWidget);
    stackedWidget.addWidget(menuWidget);
    stackedWidget.addWidget(workSpaceWidget);
    stackedWidget.setCurrentWidget(menuWidget);

    setCentralWidget(&stackedWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeWidgetToMenuWidget()
{
    stackedWidget.setCurrentWidget(menuWidget);
}

void MainWindow::on_createProjectAction_triggered()
{
    if (centralWidget() != workSpaceWidget)
    {
        stackedWidget.setCurrentWidget(workSpaceWidget);
    }
    workSpaceWidget->addProjectWindow();
}


void MainWindow::on_openProjectAction_triggered()
{
//    QString filePath =  QFileDialog::getOpenFileName(this, "Выберите файл", QDir::homePath(), "SAPR-проект (*.sapr)");
//    if (FileHandler::openProject(filePath.toStdString()))
//    {
//        workSpace.elements = FileHandler::readElements;
//        workSpace.correctLinkedElementsPos();
//        workSpace.autoSizeElements();
//    }
//    CalculationProducer::dropCalculation();
//    qDebug() << filePath;
//    update();
}


void MainWindow::on_saveProjectAction_triggered()
{
//    FileHandler::saveProject(workSpace.elements);
}

