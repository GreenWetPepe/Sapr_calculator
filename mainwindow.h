#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "calculationproducer.h"
#include "workspace.h"
#include "filehandler.h"

#include <QMainWindow>
#include <QPainter>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent *event);

private slots:

    void on_addSegment_clicked();

    void on_setData_clicked();

    void on_create_triggered();

    void on_open_triggered();

    void on_save_triggered();

    void on_buildAction_triggered();

    void on_lineEdit_editingFinished();

    void on_showTableAction_triggered();

private:
    Ui::MainWindow *ui;

    WorkSpace workSpace;
    SaprElement *selectedElement;
    QWidget *settingsWidget;
    QWidget *calcPointWidget;
    Qt::MouseButton lastButton = Qt::NoButton;
    int lastX, lastY;

    int size = 10;

    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

    void moveElementDataToSettings();
    void moveElementDataToCalcPoint();
};
#endif // MAINWINDOW_H
