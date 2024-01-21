#include "projectwidget.h"
#include "ui_projectwidget.h"

#include "Options.cpp"

#include <QDebug>
#include <QKeyEvent>
#include <QDesktopServices>
#include <QDir>

#include <random>
#include <algorithm>

std::vector<int> ProjectWidget::buttonsBuffer;

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

void ProjectWidget::addButtonToBuffer(int button)
{
    for (int btn : buttonsBuffer)
    {
        if (button == btn) return;
    }

    buttonsBuffer.push_back(button);
}

void ProjectWidget::deleteButtonFromBuffer(int button)
{
    for (int i = 0; i < buttonsBuffer.size(); i++)
    {
        if (button == buttonsBuffer[i])
        {
            buttonsBuffer.erase(buttonsBuffer.begin() + i);
            return;
        }
    }
}

void ProjectWidget::clearSelectedElements()
{
    for (auto el : selectedElements)
    {
        el->setSelected(false);
    }
    selectedElements.clear();
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
    saved = false;
    emit setTabWidgetStateName(this, saved);
    update();
}

void ProjectWidget::keyPressEvent(QKeyEvent *event)
{
    if (buttonsBuffer.size() == 1 && buttonsBuffer[0] == Qt::Key_Control)
    {
        if (event->key() == Qt::Key_S)
        {
            save();
        }

        if (event->key() == Qt::Key_V)
        {
            for (auto el : selectedElements)
            {
                workSpace.addElement(new SaprElement(*el));
                auto lastElement = workSpace.elements.back();
                lastElement->move(-150, -150);
                workSpace.checkForConnections(lastElement);
            }
            int size = selectedElements.size();
            clearSelectedElements();
            for (int i = workSpace.elements.size() - 1; i >= workSpace.elements.size() - size; i--)
            {
                selectedElements.push_back(workSpace.elements[i]);
            }
            qDebug() << selectedElements.size();
        }
    }
    else
    {
        if (event->key() == Qt::Key_W) workSpace.moveElements(0, -20);
        if (event->key() == Qt::Key_S) workSpace.moveElements(0, 20);
        if (event->key() == Qt::Key_A) workSpace.moveElements(-20, 0);
        if (event->key() == Qt::Key_D) workSpace.moveElements(20, 0);

        if (event->key() == Qt::Key_Escape)
        {
            for (auto el : selectedElements)
            {
                el->setSelected(false);
            }
            selectedElements.clear();
            emit linkSelectedElementsDataWithWidget(selectedElements);
        }

        if (event->key() == Qt::Key_Delete)
        {
            for (auto el : selectedElements)
            {
                workSpace.removeElement(el);
            }
            selectedElements.clear();
            emit linkSelectedElementsDataWithWidget(selectedElements);
        }

        if (event->key() == Qt::Key_Control)
        {
            addButtonToBuffer(event->key());
        }
    }
    update();
}

void ProjectWidget::keyReleaseEvent(QKeyEvent *event)
{
    deleteButtonFromBuffer(event->key());
}

void ProjectWidget::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) workSpace.zoomIn();
    else if (event->delta() < 0) workSpace.zoomOut();
    update();

}

void ProjectWidget::mouseMoveEvent(QMouseEvent *event)
{
    int deltX = event->x() - lastX, deltY = event->y() - lastY;
    if (deltX != 0 || deltY != 0)
    {
        if (lastMouseButton == Qt::RightButton)
        {
            workSpace.moveElements(deltX, deltY);
            isLastEventMove = true;
        }
        else if (lastMouseButton == Qt::LeftButton)
        {
            nowMouseMoveEventCall++;
            if (nowMouseMoveEventCall > options::workSpace::mouseMoveEventCallForStartMoveElement && lastMousePressElement)
            {
                for (auto el : selectedElements)
                {
                    el->move(deltX, deltY);
                    isLastEventMove = true;
                }
                workSpace.dropCalc();
            }
        }
        else
        {
            isLastEventMove = false;
            nowMouseMoveEventCall = 0;
        }
        lastX = event->x(), lastY = event->y();
    }
    update();
}

void ProjectWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePressElement = nullptr;
    isLastEventMove = false;
    nowMouseMoveEventCall = 0;
    lastMouseButton = event->button();
    lastX = event->x(), lastY = event->y();

    if (event->button() == Qt::LeftButton)
    {
        bool isElementInBuffer = false;
        int index = 0;
        lastMousePressElement = workSpace.findElement(lastX, lastY);
        if (lastMousePressElement)
        {
            prevLastMousePressElementSelection = lastMousePressElement->isSelected();
            for (int i = 0; i < selectedElements.size(); i++)
            {
                if (selectedElements[i] == lastMousePressElement)
                {
                    index = i;
                    isElementInBuffer = true;
                    break;
                }
            }

            if (buttonsBuffer.size() == 1 && buttonsBuffer[0] == Qt::Key_Control)
            {
                if (!isElementInBuffer)
                {
                    lastMousePressElement->setSelected(true);
                    selectedElements.push_back(lastMousePressElement);
                }
            }
            else
            {
                clearSelectedElements();
                lastMousePressElement->setSelected(true);
                selectedElements.push_back(lastMousePressElement);
            }
        }
        else
        {
            clearSelectedElements();
        }
        emit linkSelectedElementsDataWithWidget(selectedElements);
    }
    update();
}

void ProjectWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        bool isElementInBuffer = false;
        int index = 0;
        SaprElement *selectedElement = workSpace.findElement(lastX, lastY);
        if (selectedElement)
        {
            for (int i = 0; i < selectedElements.size(); i++)
            {
                if (selectedElements[i] == selectedElement)
                {
                    index = i;
                    isElementInBuffer = true;
                    break;
                }
            }

            if (!isLastEventMove)
            {
                if (buttonsBuffer.size() == 1 && buttonsBuffer[0] == Qt::Key_Control && isElementInBuffer && prevLastMousePressElementSelection)
                {
                    selectedElements[index]->setSelected(false);
                    selectedElements.erase(selectedElements.begin() + index);
                }
            }
        }

        if (!selectedElements.empty() && isLastEventMove)
        {
            for (auto el : selectedElements)
            {
                workSpace.checkForConnections(el);
                saved = false;
                emit setTabWidgetStateName(this, saved);
            }
        }

        emit linkSelectedElementsDataWithWidget(selectedElements);
    }


    lastMouseButton = Qt::NoButton;
    isLastEventMove = false;
    nowMouseMoveEventCall = 0;
    update();
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
    if (selectedElements.empty()) return;
    workSpace.autoSizeElements();
    workSpace.dropCalc();
    update();
    saved = false;
    emit setTabWidgetStateName(this, saved);
    emit linkSelectedElementsDataWithWidget(selectedElements);
}

std::string ProjectWidget::getProjectPath()
{
    return projectPath;
}

std::string ProjectWidget::getProjectName()
{
    return projectName;
}

bool ProjectWidget::isSaved()
{
    return saved;
}

void ProjectWidget::save()
{
    if (projectPath.empty())
    {
        QString standartFileName = "";
        QString filePath = QFileDialog::getSaveFileName(this, "Создать файл", QDir::homePath(), "SAPR-проект (*.sapr);;Все файлы (*.*)", &standartFileName);
        if (filePath == "") return;
        FileHandler::createFile(filePath.toStdString());
        changeProjectPathAndName(filePath.toStdString());
        update();
    }
    else
    {
        FileHandler::saveProject(projectPath, workSpace.elements);
    }
    saved = true;
    emit setTabWidgetStateName(this, saved);
}

void ProjectWidget::openProject()
{

    QString filePath = QFileDialog::getOpenFileName(this, "Выберите файл", QDir::homePath(), "SAPR-проект (*.sapr)");
    if (filePath == "") return;
    if (FileHandler::openProject(filePath.toStdString()))
    {
        changeProjectPathAndName(filePath.toStdString());
        workSpace.elements = FileHandler::readElements;
        workSpace.correctLinkedElementsPos();
        workSpace.autoSizeElements();
        saved = true;
        emit setTabWidgetStateName(this, saved);
        CalculationProducer::dropCalculation();
    }
    update();
}

void ProjectWidget::changeProjectPathAndName(std::string path)
{
    projectName = "";
    projectPath = path;
    for (int i = path.size() - 1; i >= 0; i--)
    {
        if (path[i] == *"/") break;
        projectName += path[i];
    }

    std::reverse(projectName.begin(), projectName.end());
}

void ProjectWidget::calcGraph()
{
    workSpace.calc();
    update();
}

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
