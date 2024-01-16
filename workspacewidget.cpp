#include "workspacewidget.h"
#include "ui_workspacewidget.h"

#include <QDebug>

WorkSpaceWidget::WorkSpaceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkSpaceWidget)
{
    ui->setupUi(this);
    projectWidgets.push_back(new ProjectWidget(ui->tab));
    connect(projectWidgets[0], &ProjectWidget::linkSelectedElementsDataWithWidget,
            this, &WorkSpaceWidget::setElementParameters);
}

WorkSpaceWidget::~WorkSpaceWidget()
{
    delete ui;
}

void WorkSpaceWidget::resizeEvent(QResizeEvent *event)
{
//    qDebug() << "Resize event (workspace) " << ui->tabWidget->size();
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

void WorkSpaceWidget::sendElementParametersToProjectWidget()
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

    projectWidgets[tabIndex]->setSelectedElementsParameters(elementData);
}

void WorkSpaceWidget::on_lengthLE_editingFinished()
{
    sendElementParametersToProjectWidget();
}


void WorkSpaceWidget::on_squareLE_editingFinished()
{
    sendElementParametersToProjectWidget();
}


void WorkSpaceWidget::on_elasticLE_editingFinished()
{
    sendElementParametersToProjectWidget();
}


void WorkSpaceWidget::on_stressLE_editingFinished()
{
    sendElementParametersToProjectWidget();
}


void WorkSpaceWidget::on_xLeftLE_editingFinished()
{
    sendElementParametersToProjectWidget();
}


void WorkSpaceWidget::on_xRightLE_editingFinished()
{
    sendElementParametersToProjectWidget();
}


void WorkSpaceWidget::on_qxLE_editingFinished()
{
    sendElementParametersToProjectWidget();
}


void WorkSpaceWidget::on_leftSupCB_stateChanged(int arg1)
{
    sendElementParametersToProjectWidget();
}


void WorkSpaceWidget::on_rightSupCB_stateChanged(int arg1)
{
    sendElementParametersToProjectWidget();
}

