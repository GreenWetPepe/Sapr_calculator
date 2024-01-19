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

    setCentralWidget(menuWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_createProjectAction_triggered()
{
    if (centralWidget() != workSpaceWidget)
    {
        setCentralWidget(workSpaceWidget);
    }
    workSpaceWidget->addProjectWindow();
//    QString standartFileName = "";
//    QString filePath = QFileDialog::getSaveFileName(this, "Создать файл", QDir::homePath(), "SAPR-проект (*.sapr);;Все файлы (*.*)", &standartFileName);
//    CalculationProducer::dropCalculation();
//    FileHandler::createFile(filePath.toStdString());
//    workSpace.elements.clear();
//    qDebug() << filePath;
//    update();

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

