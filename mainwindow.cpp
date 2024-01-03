#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QKeyEvent>
#include <random>
#include <QDesktopServices>
#include <QDir>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    qSetRealNumberPrecision(10);
    selectedElement = nullptr;
    ui->setupUi(this);

    workSpace.setWindowSize(centralWidget()->size().width(), centralWidget()->size().height());
    settingsWidget = centralWidget()->findChild<QWidget*>("verticalLayoutWidget");
    settingsWidget->setStyleSheet("background-color: white");
    settingsWidget->setContentsMargins(8, 8, 8, 8);
    settingsWidget->hide();

    calcPointWidget = centralWidget()->findChild<QWidget*>("calcPoint");
    calcPointWidget->setStyleSheet("background-color: white");
    calcPointWidget->setContentsMargins(8, 8, 8, 8);
    calcPointWidget->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    workSpace.drawElements(painter);
    workSpace.drawDiagram(painter);
}


void MainWindow::on_addSegment_clicked()
{
    workSpace.addElement(new SaprElement());
    workSpace.autoSizeElements();
    update();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W) workSpace.moveElements(0, -20);
    if (event->key() == Qt::Key_S) workSpace.moveElements(0, 20);
    if (event->key() == Qt::Key_A) workSpace.moveElements(-20, 0);
    if (event->key() == Qt::Key_D) workSpace.moveElements(20, 0);

    if (selectedElement)
    {
        if (event->key() == Qt::Key_V)
        {
            workSpace.addElement(new SaprElement(*selectedElement));
            auto lastElement = workSpace.elements.back();
            lastElement->x -= lastElement->width / 2;
            lastElement->y -= lastElement->height * 1.25;
        }

        else if (event->key() == Qt::Key_Control)
        {
            selectedElement->isSelected = false;
            if (selectedElement->rightConnectedElement)
            {
                selectedElement = selectedElement->rightConnectedElement;
            }
            else if (workSpace.checkSystemReadiness())
            {
                selectedElement = workSpace.getFirstLinkedElement();
            }
            selectedElement->isSelected = true;
            moveElementDataToSettings();
            moveElementDataToCalcPoint();
            update();
        }
    }

    if (event->key() == Qt::Key_Escape)
    {
        if (selectedElement != nullptr) selectedElement->isSelected = false;
        selectedElement = nullptr;
        settingsWidget->hide();
        calcPointWidget->hide();
    }

    if (event->key() == Qt::Key_Delete)
    {
        workSpace.removeElement(selectedElement);
        selectedElement = nullptr;
        settingsWidget->hide();
        calcPointWidget->hide();
    }
    update();
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) workSpace.zoomIn();
    else if (event->delta() < 0) workSpace.zoomOut();
    update();

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (lastButton == Qt::RightButton)
    {
        workSpace.moveElements(event->x() - lastX, event->y() - lastY);
        update();
    }
    else if (lastButton == Qt::LeftButton)
    {
        if (selectedElement != nullptr)
        {
            selectedElement->x += event->x() - lastX;
            selectedElement->y += event->y() - lastY;
            workSpace.dropCalc();
            update();
        }
    }
    lastX = event->x(), lastY = event->y();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (lastButton == Qt::NoButton && (selectedElement == nullptr ||
        event->x() < settingsWidget->x() || event->y() < settingsWidget->y() ||
        event->x() > settingsWidget->x() + settingsWidget->width() || event->y() > settingsWidget->y() + settingsWidget->height()))
    {
        lastButton = event->button();
        lastX = event->x(), lastY = event->y();

        if (lastButton == Qt::LeftButton)
        {
            if (selectedElement != nullptr)
            {
                selectedElement->isSelected = false;
                settingsWidget->hide();
                calcPointWidget->hide();
            }
            selectedElement = workSpace.findElement(lastX, lastY);
            if (selectedElement != nullptr)
            {
                settingsWidget->show();
                selectedElement->isSelected = true;
                if (CalculationProducer::isReady())
                {
                    calcPointWidget->show();
                    moveElementDataToCalcPoint();
                }
                moveElementDataToSettings();
            }
        }
    }
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (lastButton == Qt::LeftButton && selectedElement != nullptr)
    {
        workSpace.checkForConnection(selectedElement);
        update();
    }
    lastButton = Qt::NoButton;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    workSpace.setWindowSize(event->size().width(), event->size().height());
    workSpace.autoSizeElements();
    if (settingsWidget != nullptr)
    {
        settingsWidget->setGeometry(event->size().width() - settingsWidget->width() - 40, event->size().height() - settingsWidget->height() - 60,
                                    settingsWidget->width(), settingsWidget->height());
    }
    if (calcPointWidget != nullptr)
    {
        calcPointWidget->setGeometry(calcPointWidget->x(), event->size().height() - calcPointWidget->height() - 60,
                                    calcPointWidget->width(), calcPointWidget->height());
    }
    update();
}

void MainWindow::moveElementDataToSettings()
{
    auto settings = centralWidget()->findChildren<QLineEdit*>();
    auto supSettings = centralWidget()->findChildren<QCheckBox*>();
    settings[0]->setText(QString::number(selectedElement->length));
    settings[1]->setText(QString::number(selectedElement->square));
    settings[2]->setText(QString::number(selectedElement->elasticModulus));
    settings[3]->setText(QString::number(selectedElement->permissibleStress));
    settings[4]->setText(QString::number(selectedElement->xLeftForce));
    settings[5]->setText(QString::number(selectedElement->xRightForce));
    settings[6]->setText(QString::number(selectedElement->xQForce));
    supSettings[0]->setChecked(selectedElement->hasRightSupport);
    supSettings[1]->setChecked(selectedElement->hasLeftSupport);
}

void MainWindow::moveElementDataToCalcPoint()
{
    auto xParam = calcPointWidget->findChild<QLineEdit*>();
    auto labels = calcPointWidget->findChildren<QLabel*>();
    labels[0]->setText(QString("sX = "));
    labels[1]->setText(QString("uX = "));
    labels[2]->setText(QString("nX = "));
    labels[3]->setText((QString("Введите длину от 0 до ") + QString::number(selectedElement->length)));
}


void MainWindow::on_setData_clicked()
{
    auto settings = settingsWidget->findChildren<QLineEdit*>();
    auto supSettings = settingsWidget->findChildren<QCheckBox*>();
    if (selectedElement == nullptr) return;
    try
    {
        if (settings[0]->text().toDouble() > 0) selectedElement->length = settings[0]->text().toDouble();
        if (settings[1]->text().toDouble() > 0) selectedElement->square = settings[1]->text().toDouble();
        if (settings[2]->text().toDouble() > 0) selectedElement->elasticModulus = settings[2]->text().toDouble();
        if (settings[3]->text().toDouble() > 0) selectedElement->permissibleStress = settings[3]->text().toDouble();
        selectedElement->xLeftForce = settings[4]->text().toDouble();
        if (selectedElement->leftConnectedElement != nullptr) selectedElement->leftConnectedElement->xRightForce = selectedElement->xLeftForce;
        selectedElement->xRightForce = settings[5]->text().toDouble();
        if (selectedElement->rightConnectedElement != nullptr) selectedElement->rightConnectedElement->xLeftForce = selectedElement->xRightForce;
        selectedElement->xQForce = settings[6]->text().toDouble();
        if (selectedElement->rightConnectedElement == nullptr) selectedElement->hasRightSupport = supSettings[0]->isChecked();
        if (selectedElement->leftConnectedElement == nullptr) selectedElement->hasLeftSupport = supSettings[1]->isChecked();
        workSpace.autoSizeElements();
        workSpace.dropCalc();
        update();
    } catch (...)
    {

    }
}


void MainWindow::on_create_triggered()
{
    QString standartFileName = "";
    QString filePath = QFileDialog::getSaveFileName(this, "Создать файл", QDir::homePath(), "SAPR-проект (*.sapr);;Все файлы (*.*)", &standartFileName);
    CalculationProducer::dropCalculation();
    FileHandler::createFile(filePath.toStdString());
    workSpace.elements.clear();
    qDebug() << filePath;
    update();
}


void MainWindow::on_open_triggered()
{
    QString filePath =  QFileDialog::getOpenFileName(this, "Выберите файл", QDir::homePath(), "SAPR-проект (*.sapr)");
    if (FileHandler::openProject(filePath.toStdString()))
    {
        workSpace.elements = FileHandler::readElements;
        workSpace.correctLinkedElementsPos();
        workSpace.autoSizeElements();
    }
    CalculationProducer::dropCalculation();
    qDebug() << filePath;
    update();
}


void MainWindow::on_save_triggered()
{
    FileHandler::saveProject(workSpace.elements);
}


void MainWindow::on_buildAction_triggered()
{
    workSpace.calc();
    update();
}

void MainWindow::on_lineEdit_editingFinished()
{
    if (!workSpace.checkSystemReadiness()) return;
    auto xParam = calcPointWidget->findChild<QLineEdit*>();
    auto labels = calcPointWidget->findChildren<QLabel*>();
    double val = xParam->text().toDouble();
    if (val < 0 || val > selectedElement->length) return;
    SaprElement *firstElement = workSpace.getFirstLinkedElement();
    int i = 0;
    while (firstElement)
    {
        if (selectedElement == firstElement) break;
        firstElement = firstElement->rightConnectedElement;
        i++;
    }
    CalculationProducer::calculateArguments(workSpace.elements);
    double *res = CalculationProducer::calcPoint(selectedElement, i, val);
    labels[0]->setText((QString("sX = ") + QString::number(res[2])));
    labels[1]->setText((QString("uX = ") + QString::number(res[1])));
    labels[2]->setText((QString("nX = ") + QString::number(res[0])));
}


void MainWindow::on_showTableAction_triggered()
{
    if (!workSpace.checkSystemReadiness()) return;
}

