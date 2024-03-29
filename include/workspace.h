#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "engine/calculationproducer.h"
#include "engine/saprelement.h"

#include <vector>
#include <QDir>


class WorkSpace
{
public:
    WorkSpace();
    WorkSpace(std::string);
//    ~WorkSpace();

    void addElement(SaprElement *element);
    void removeElement(SaprElement *element);
    int getMaxHeight();

    SaprElement* findElement(int x, int y);
    SaprElement* getFirstLinkedElement();

    void zoomIn();
    void zoomOut();
    void moveElements(int deltX, int deltY);
    void autoSizeElements();
    void correctLinkedElementsPos();
    void checkForConnections(SaprElement *element);
    bool checkSystemReadiness();

    void drawElements(QPainter &painter);
    void drawSupports(QPainter &painter, int maxHeight);
    void drawDiagram(QPainter &painter);


    void calc();
    void dropCalc();


    void setWindowSize(int width, int height);

    std::vector<SaprElement*> elements;

private:
    int windowWidth, windowHeight;

    const double minSizeMult = 0.4;

    double sizeMult = 1;

    std::vector<std::vector<std::vector<double>>> calculation;
};

#endif // WORKSPACE_H
