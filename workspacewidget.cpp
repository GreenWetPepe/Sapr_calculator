#include "workspacewidget.h"
#include "ui_workspacewidget.h"

#include "projectwidget.h"

WorkSpaceWidget::WorkSpaceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkSpaceWidget)
{
    ui->setupUi(this);
    ProjectWidget *projectWidget = new ProjectWidget(ui->tab);
}

WorkSpaceWidget::~WorkSpaceWidget()
{
    delete ui;
}
