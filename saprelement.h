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
    void drawArrow(QPainter&, double, double, double, double);
    void drawArrowHead(QPainter&, double, double, bool);
    void drawParams(QPainter&, double);
    void drawDiagram(QPainter&, std::vector<double*>, int, double, double**);
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

private:
    static const int qForceArrowIndent = 10;
    static const int xQForceArrowIndent = 7, yQForceArrowIndent = 5;
    constexpr static double forceArrowIndentCoeff = 0.4;

    static double sizeMultiply;
    static double paramIndentMultiply;
    static double diagramSizeMultiply;
    static double diagramIndentMultiply;
};

#endif // SAPRELEMENT_H
