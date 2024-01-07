#ifndef SAPRELEMENT_H
#define SAPRELEMENT_H

#include<iostream>
#include<QPainter>

class SaprElement
{
public:
    SaprElement();
    SaprElement(double length, double square, double elasticModulus, double permissibleStress, double xLeftForce, double xRightForce, double xQForce, bool hasLeftSup, bool hasRightSup);
    SaprElement(const SaprElement &el);

    void draw(QPainter &painter, double maxHeight);
    void drawForces(QPainter &painter);
    void drawDisturbedForces(QPainter &painter);
    void drawArrow(QPainter &painter, int stX, int stY, int endX, int endY);
    void drawArrowHead(QPainter &painter, int x, int y, bool isDirectLeft);
    void drawParams(QPainter &painter, int maxHeight);
    void drawDiagram(QPainter &painter, std::vector<double> points, int maxHeight, double minVal, double maxVal, int indent);
    void zoom(double multiply);

    void setLeftForce(double force);
    void setRightForce(double force);
    void setRightConnection(SaprElement *element);
    void setLeftConnection(SaprElement *element);

    QString doubleToQString(double num);

    int x = 100, y = 100;
    double width = 100, height = 100;
    double length = 1, square = 1, elasticModulus = 1, permissibleStress = 1;
    double xRightForce = 0, xLeftForce = 0, xQForce = 0;
    bool isSelected = false;
    bool hasLeftSupport = false, hasRightSupport = false;

    SaprElement *leftConnectedElement = nullptr, *rightConnectedElement = nullptr;
};

#endif // SAPRELEMENT_H
