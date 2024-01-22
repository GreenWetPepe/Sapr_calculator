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
    void switchCalcPointWidgetPos();

    std::string getProjectPath();
    std::string getProjectName();
    bool isSaved();
    void save();
    void openProject();

    void calcGraph();


signals:
    void linkSelectedElementsDataWithWidget(std::vector<SaprElement*> selectedElements);
    void setTabWidgetStateName(ProjectWidget *projectWidget, bool isSaved);

private slots:

    void on_addSegment_clicked();

    void on_pointCalcWindowButton_clicked();

    void on_xLE_editingFinished();

private:
    Ui::ProjectWidget *ui;

    WorkSpace workSpace;
    std::vector<SaprElement*> selectedElements;
    int lastX, lastY;
    int nowMouseMoveEventCall = 0;


    int lastMouseButton = Qt::NoButton;
    bool isLastEventMove = false;
    SaprElement *lastMousePressElement = nullptr;
    bool prevLastMousePressElementSelection = false;
    static std::vector<int> buttonsBuffer;

    std::string projectPath = "";
    std::string projectName = "untitled.sapr";
    bool saved = true;

    bool isPointCalcWidgetOpen = true;

    void addButtonToBuffer(int button);
    void deleteButtonFromBuffer(int button);

    void clearSelectedElements();
    void changeProjectPathAndName(std::string path);

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
