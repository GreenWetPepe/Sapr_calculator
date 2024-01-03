#ifndef SAPRELEMENT_H
#define SAPRELEMENT_H

#include<iostream>
#include<QPainter>

class SaprElement
{
public:
    SaprElement();
    SaprElement(int, int, double width = 50, double height = 50);
    SaprElement(double, double, double, double, double, double, double);
    SaprElement(double, double, double, double, double, double, double, bool, bool);
    SaprElement(const SaprElement&);
    SaprElement(std::string);

    void draw(QPainter&, double);
    void drawForces(QPainter &painter);
    void drawDisturbedForces(QPainter &painter);
    void drawArrow(QPainter &painter, int stX, int stY, int endX, int endY);
    void drawArrowHead(QPainter &painter, int x, int y, bool isDirectLeft);
    void drawParams(QPainter &painter, int maxHeight);
    void drawDiagram(QPainter &painter, std::vector<double> points, int maxHeight, double minVal, double maxVal, int indent);
    void zoom(double);

    void setLeftForce(double);
    void setRightForce(double);
    void setRightConnection(SaprElement*);
    void setLeftConnection(SaprElement*);

    QString doubleToQString(double);

    int x = 100, y = 100;
    double width = 100, height = 100;
    double length = 1, square = 1, elasticModulus = 1, permissibleStress = 1;
    double xRightForce = 0, xLeftForce = 0, xQForce = 0;
    bool isSelected = false;
    bool hasLeftSupport = false, hasRightSupport = false;

    SaprElement *leftConnectedElement = nullptr, *rightConnectedElement = nullptr;
};

#endif // SAPRELEMENT_H
