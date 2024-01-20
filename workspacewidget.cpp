#include "workspacewidget.h"
#include "ui_workspacewidget.h"

#include <QDebug>

WorkSpaceWidget::WorkSpaceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkSpaceWidget)
{
    ui->setupUi(this);
}

WorkSpaceWidget::~WorkSpaceWidget()
{
    delete ui;
}

void WorkSpaceWidget::addProjectWindow()
{
    projectWidgets.push_back(new ProjectWidget());
    ui->tabWidget->addTab(projectWidgets.back(), QString::fromStdString(projectWidgets.back()->getProjectName()));
    projectWidgets.back()->resizeWidget(ui->tabWidget->size().width(), ui->tabWidget->size().height());
    connect(projectWidgets.back(), &ProjectWidget::linkSelectedElementsDataWithWidget,
            this, &WorkSpaceWidget::setElementParameters);
    connect(projectWidgets.back(), &ProjectWidget::setTabWidgetStateName,
            this, &WorkSpaceWidget::markTabAsSaveState);
}

void WorkSpaceWidget::markTabAsSaveState(ProjectWidget *projectWidget, bool isSaved)
{
    for (int i = 0; i < projectWidgets.size(); i++)
    {
        if (projectWidgets[i] == projectWidget)
        {
            if (!isSaved)
            {
                ui->tabWidget->setTabText(i, QString::fromStdString(projectWidget->getProjectName() + "*"));
            }
            else
            {
                ui->tabWidget->setTabText(i, QString::fromStdString(projectWidget->getProjectName()));
            }
        }
    }
}

void WorkSpaceWidget::resizeEvent(QResizeEvent *event)
{
    for (auto projectWidget : projectWidgets)
    {
        projectWidget->resizeWidget(ui->tabWidget->size().width(), ui->tabWidget->size().height());
    }
}



void WorkSpaceWidget::setElementParameters(std::vector<SaprElement*> selectedElements)
{
    if (selectedElements.empty())
    {
        clearElementParameters();
        return;
    }
    SaprElement *el = selectedElements.back();
    ui->lengthLE->setText(QString::number(el->getLength()));
    ui->squareLE->setText(QString::number(el->getSquare()));
    ui->elasticLE->setText(QString::number(el->getElasticModulus()));
    ui->stressLE->setText(QString::number(el->getPermissibleStress()));
    ui->xLeftLE->setText(QString::number(el->getXLeftForce()));
    ui->xRightLE->setText(QString::number(el->getXRightForce()));
    ui->qxLE->setText(QString::number(el->getXQForce()));
    ui->leftSupCB->setChecked(el->hasLeftSupport());
    ui->rightSupCB->setChecked(el->hasRightSupport());
}

void WorkSpaceWidget::clearElementParameters()
{
    ui->lengthLE->setText(QString(""));
    ui->squareLE->setText(QString(""));
    ui->elasticLE->setText(QString(""));
    ui->stressLE->setText(QString(""));
    ui->xLeftLE->setText(QString(""));
    ui->xRightLE->setText(QString(""));
    ui->qxLE->setText(QString(""));
    ui->leftSupCB->setChecked(false);
    ui->rightSupCB->setChecked(false);
}

void WorkSpaceWidget::sendElementParametersToProjectWidget(std::vector<int> dataMask)
{
    int tabIndex = ui->tabWidget->currentIndex();
    if (tabIndex == -1)
    {
        clearElementParameters();
        return;
    }

    SaprElementData elementData;
    elementData.length = ui->lengthLE->text().toDouble();
    elementData.square = ui->squareLE->text().toDouble();
    elementData.elasticModulus = ui->elasticLE->text().toDouble();
    elementData.permissibleStress = ui->stressLE->text().toDouble();
    elementData.xLeftForce = ui->xLeftLE->text().toDouble();
    elementData.xRightForce = ui->xRightLE->text().toDouble();
    elementData.xQForce = ui->qxLE->text().toDouble();
    elementData.hasLeftSupport = ui->leftSupCB->checkState();
    elementData.hasRightSupport = ui->rightSupCB->checkState();
    elementData.maskInit(dataMask);

    projectWidgets[tabIndex]->setSelectedElementsParameters(elementData);
}

void WorkSpaceWidget::on_tabWidget_tabCloseRequested(int index)
{
    if (!projectWidgets[index]->isSaved())
    {
        projectWidgets[index]->save();
    }

    delete projectWidgets[index];
    projectWidgets.erase(projectWidgets.begin() + index);
    ui->tabWidget->removeTab(index);
    if (ui->tabWidget->currentIndex() == -1)
    {
        emit changeMainWindowCentailWidgetToMenu();
    }
}

void WorkSpaceWidget::on_lengthLE_editingFinished()
{
    sendElementParametersToProjectWidget(std::vector<int>({0}));
}


void WorkSpaceWidget::on_squareLE_editingFinished()
{
    sendElementParametersToProjectWidget(std::vector<int>({1}));
}


void WorkSpaceWidget::on_elasticLE_editingFinished()
{
    sendElementParametersToProjectWidget(std::vector<int>({2}));
}


void WorkSpaceWidget::on_stressLE_editingFinished()
{
    sendElementParametersToProjectWidget(std::vector<int>({3}));
}


void WorkSpaceWidget::on_xLeftLE_editingFinished()
{
    sendElementParametersToProjectWidget(std::vector<int>({4}));
}


void WorkSpaceWidget::on_xRightLE_editingFinished()
{
    sendElementParametersToProjectWidget(std::vector<int>({5}));
}


void WorkSpaceWidget::on_qxLE_editingFinished()
{
    sendElementParametersToProjectWidget(std::vector<int>({6}));
}


void WorkSpaceWidget::on_leftSupCB_stateChanged(int arg1)
{
    sendElementParametersToProjectWidget(std::vector<int>({7}));
}


void WorkSpaceWidget::on_rightSupCB_stateChanged(int arg1)
{
    sendElementParametersToProjectWidget(std::vector<int>({8}));
}

