#include "projectwidget.h"
#include "ui_projectwidget.h"

#include <QDebug>
#include <QKeyEvent>
#include <random>
#include <QDesktopServices>
#include <QDir>

ProjectWidget::ProjectWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ProjectWidget)
{
    qSetRealNumberPrecision(10);
    ui->setupUi(this);

    workSpace.setWindowSize(200, 200);
}

ProjectWidget::~ProjectWidget()
{
    delete ui;
}

void ProjectWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    workSpace.drawElements(painter);
    workSpace.drawDiagram(painter);
}


void ProjectWidget::on_addSegment_clicked()
{
    workSpace.addElement(new SaprElement());
    workSpace.autoSizeElements();
    update();
}

void ProjectWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W) workSpace.moveElements(0, -20);
    if (event->key() == Qt::Key_S) workSpace.moveElements(0, 20);
    if (event->key() == Qt::Key_A) workSpace.moveElements(-20, 0);
    if (event->key() == Qt::Key_D) workSpace.moveElements(20, 0);

    if (!selectedElements.empty())
    {
        if (event->key() == Qt::Key_V)
        {
            for (auto el : selectedElements)
            {
                workSpace.addElement(new SaprElement(*el));
                auto lastElement = workSpace.elements.back();
                lastElement->move(-100, -100);
            }
        }
    }

    if (event->key() == Qt::Key_Escape)
    {
        for (auto el : selectedElements)
        {
            el->setSelected(false);
        }
        selectedElements.clear();
        emit linkSelectedElementsDataWithWidget(selectedElements);
//        settingsWidget->hide();
//        calcPointWidget->hide();
    }

    if (event->key() == Qt::Key_Delete)
    {
        for (auto el : selectedElements)
        {
            workSpace.removeElement(el);
        }
        selectedElements.clear();
        emit linkSelectedElementsDataWithWidget(selectedElements);
//        settingsWidget->hide();
//        calcPointWidget->hide();
    }
    update();
}

void ProjectWidget::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) workSpace.zoomIn();
    else if (event->delta() < 0) workSpace.zoomOut();
    update();

}

void ProjectWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (lastButton == Qt::RightButton)
    {
        workSpace.moveElements(event->x() - lastX, event->y() - lastY);
    }
    else if (lastButton == Qt::LeftButton)
    {
        for (auto el : selectedElements)
        {
            el->move(event->x() - lastX, event->y() - lastY);
        }
        workSpace.dropCalc();
    }
    lastX = event->x(), lastY = event->y();
    update();
}

void ProjectWidget::mousePressEvent(QMouseEvent *event)
{
    lastButton = event->button();
    lastX = event->x(), lastY = event->y();

    if (lastButton == Qt::LeftButton)
    {
        for (auto el : selectedElements)
        {
            el->setSelected(false);
        }
        selectedElements.clear();
        emit linkSelectedElementsDataWithWidget(selectedElements);
//      settingsWidget->hide();
//      calcPointWidget->hide();

        bool isSameElement = false;
        SaprElement *selectedElement;
        selectedElement = workSpace.findElement(lastX, lastY);
        if (selectedElement)
        {
            for (auto el : selectedElements)
            {
                if (el == selectedElement)
                {
                    isSameElement = true;
                    break;
                }
            }
            if (!isSameElement) selectedElements.push_back(selectedElement);
            emit linkSelectedElementsDataWithWidget(selectedElements);
        }

        if (selectedElement)
        {
//                settingsWidget->show();
            selectedElement->setSelected(true);
            if (CalculationProducer::isReady())
            {
//                    calcPointWidget->show();
//                    moveElementDataToCalcPoint();
            }
//                moveElementDataToSettings();
        }
    }
    update();
}

void ProjectWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (lastButton == Qt::LeftButton && !selectedElements.empty())
    {
        for (auto el : selectedElements)
        {
            workSpace.checkForConnections(el);
        }
        update();
    }
    lastButton = Qt::NoButton;
}

void ProjectWidget::resizeWidget(int width, int height)
{
    resize(width, height);
    workSpace.setWindowSize(width, height);
    workSpace.autoSizeElements();
    update();
}

void ProjectWidget::setSelectedElementsParameters(SaprElementData data)
{
    for (auto selectedElement : selectedElements)
    {
        selectedElement->setData(data);
    }
    update();
    emit linkSelectedElementsDataWithWidget(selectedElements);
}

//void ProjectWidget::on_create_triggered()
//{
//    QString standartFileName = "";
//    QString filePath = QFileDialog::getSaveFileName(this, "Создать файл", QDir::homePath(), "SAPR-проект (*.sapr);;Все файлы (*.*)", &standartFileName);
//    CalculationProducer::dropCalculation();
//    FileHandler::createFile(filePath.toStdString());
//    workSpace.elements.clear();
//    qDebug() << filePath;
//    update();
//}


//void ProjectWidget::on_open_triggered()
//{
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
//}


//void ProjectWidget::on_save_triggered()
//{
//    FileHandler::saveProject(workSpace.elements);
//}


//void ProjectWidget::on_buildAction_triggered()
//{
//    workSpace.calc();
//    update();
//}

//void ProjectWidget::on_lineEdit_editingFinished()
//{
//    if (!workSpace.checkSystemReadiness()) return;
//    auto xParam = calcPointWidget->findChild<QLineEdit*>();
//    auto labels = calcPointWidget->findChildren<QLabel*>();
//    double val = xParam->text().toDouble();
//    if (val < 0 || val > selectedElement->length) return;
//    SaprElement *firstElement = workSpace.getFirstLinkedElement();
//    int i = 0;
//    while (firstElement)
//    {
//        if (selectedElement == firstElement) break;
//        firstElement = firstElement->rightConnectedElement;
//        i++;
//    }
//    CalculationProducer::calculateArguments(workSpace.elements);
//    double *res = CalculationProducer::calcPoint(selectedElement, i, val);
//    labels[0]->setText((QString("sX = ") + QString::number(res[2])));
//    labels[1]->setText((QString("uX = ") + QString::number(res[1])));
//    labels[2]->setText((QString("nX = ") + QString::number(res[0])));
//}


//void ProjectWidget::on_showTableAction_triggered()
//{
//    if (!workSpace.checkSystemReadiness()) return;
//}
