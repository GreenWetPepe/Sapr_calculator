#ifndef SAPRELEMENT_H
#define SAPRELEMENT_H

#include "SaprElementData.h"

#include<iostream>
#include<QPainter>

class SaprElement
{
public:
    SaprElement();
    SaprElement(double length, double square, double elasticModulus, double permissibleStress, double xLeftForce, double xRightForce, double xQForce, bool hasLeftSup, bool hasRightSup);
    SaprElement(const SaprElement &el);

    void draw(QPainter &painter, double maxHeight);
    void drawDiagram(QPainter &painter, std::vector<double> points, int maxHeight, double minVal, double maxVal, int indent, std::string label);
    void drawSupports(QPainter &painter, int maxHeight);
    void zoom(double multiply);

    void adjustSize(double maxLength, double maxSquare, int windowWidth, int windowHeight, double sizeMult);
    void move(int x, int y);
    void correctPosToLinkedElement();
    int checkForConnection(SaprElement *element);

    void setData(SaprElementData data);
    void setLeftForce(double force);
    void setRightForce(double force);
    void setRightConnection(SaprElement *element);
    void setLeftConnection(SaprElement *element);

    SaprElementData getData();
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    double getLength();
    double getSquare();
    double getElasticModulus();
    double getPermissibleStress();
    double getXLeftForce();
    double getXRightForce();
    double getXQForce();
    bool hasLeftSupport();
    bool hasRightSupport();
    bool isSelected();
    SaprElement* getLeftConnectedElement();
    SaprElement* getRightConnectedElement();

    void setLength(double length);
    void setSquare(double square);
    void setElasticModulus(double elasticModulus);
    void setPermissibleStress(double permissibleStress);
    void setXQForce(double xQForce);
    void setLeftSupport(bool leftSupport);
    void setRightSupport(bool rightSupport);
    void setSelected(bool selected);

    QString doubleToQString(double num);

private:
    int x = 100, y = 100;
    double width = 100, height = 100;
    double length = 1, square = 1, elasticModulus = 1, permissibleStress = 1;
    double xLeftForce = 0, xRightForce = 0, xQForce = 0;
    bool selected = false;
    bool leftSupport = false, rightSupport = false;

    SaprElement *leftConnectedElement = nullptr, *rightConnectedElement = nullptr;

    void drawForces(QPainter &painter);
    void drawDisturbedForces(QPainter &painter);
    void drawArrow(QPainter &painter, int stX, int stY, int endX, int endY);
    void drawArrowHead(QPainter &painter, int x, int y, bool isDirectLeft);
    void drawParams(QPainter &painter, int maxHeight);
};

#endif // SAPRELEMENT_H
