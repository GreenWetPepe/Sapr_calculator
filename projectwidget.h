#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include "calculationproducer.h"
#include "workspace.h"
#include "filehandler.h"
#include "SaprElementData.h"

#include <QWidget>
#include <QPainter>
#include <QFileDialog>


namespace Ui {
class ProjectWidget;
}

class ProjectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectWidget(QWidget *parent = nullptr);
    ~ProjectWidget();

    virtual void paintEvent(QPaintEvent *event);

    void resizeWidget(int width, int height);
    void setSelectedElementsParameters(SaprElementData data);


signals:
    void linkSelectedElementsDataWithWidget(std::vector<SaprElement*> selectedElements);

private slots:

    void on_addSegment_clicked();

//    void on_create_triggered();

//    void on_open_triggered();

//    void on_save_triggered();

//    void on_buildAction_triggered();

//    void on_lineEdit_editingFinished();

//    void on_showTableAction_triggered();

private:
    Ui::ProjectWidget *ui;

    WorkSpace workSpace;
    std::vector<SaprElement*> selectedElements;
    int lastX, lastY;

    int size = 10;

    int lastMouseButton = Qt::NoButton;
    bool isLastEventMove = false;
    SaprElement *lastMousePressElement = nullptr;
    bool prevLastMousePressElementSelection = false;
    static std::vector<int> buttonsBuffer;

    void addButtonToBuffer(int button);
    void deleteButtonFromBuffer(int button);

    void clearSelectedElements();

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

//    void moveElementDataToSettings();
//    void moveElementDataToCalcPoint();
};

#endif // PROJECTWIDGET_H
