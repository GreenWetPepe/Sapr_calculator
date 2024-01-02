#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "calculationproducer.h"
#include "saprelement.h"

#include <vector>
#include <QDir>


class WorkSpace
{
public:
    WorkSpace();
    WorkSpace(std::string);
//    ~WorkSpace();

    void addElement(SaprElement*);
    void removeElement(SaprElement*);
    SaprElement* findElement(int, int);
    SaprElement* getById(int);
    void zoomIn();
    void zoomOut();
    void moveElements(int, int);
    void autoSizeElements();
    void correctLinkedElementsPos();
    void checkForConnection(SaprElement*);
    bool checkSystemReadiness();
    SaprElement* getFirstSystemElement();

    void drawElements(QPainter&);
    void drawSupports(QPainter&, int);
    void drawDiagram(QPainter&);


    void calc();
    void dropCalc();


    void setWindowSize(int, int);

    std::vector<SaprElement*> elements;

private:
    std::string projectPath = (QDir::homePath()).toStdString() + "untitled.sapr";
    int windowWidth, windowHeight;

    const int xElementConnectionSpread = 15;
    const int yElementConnectionSpread = 100;
    const double maxElementHeightRelationToWindow = 0.3;
    const double maxElementWidthRelationToWindow = 0.3;
    const double minElementHeightRalationToMaxHeight = 0.35;
    const double minElementWidthRalationToMaxWidth = 0.35;

    const double minSizeMult = 0.4;

    double sizeMult = 1;
};

#endif // WORKSPACE_H
