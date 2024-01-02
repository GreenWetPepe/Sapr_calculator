#include "saprelement.h"

#include <QDebug>

double SaprElement::sizeMultiply = 1;
double SaprElement::paramIndentMultiply = 0.7;
double SaprElement::diagramSizeMultiply = 1.2;
double SaprElement::diagramIndentMultiply = 0.45;

SaprElement::SaprElement()
{

}

SaprElement::SaprElement(int x, int y, double width, double height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    xRightForce = 0;
    xLeftForce = 0;
    xQForce = 0;
}

SaprElement::SaprElement(const SaprElement &el)
{
    x = el.x;
    y = el.y;
    width = el.width;
    height = el.height;
    length = el.length;
    square = el.square;
    elasticModulus = el.elasticModulus;
    permissibleStress = el.permissibleStress;
    xRightForce = el.xRightForce;
    xLeftForce = el.xLeftForce;
    xQForce = el.xQForce;
    isSelected = el.isSelected;
}

SaprElement::SaprElement(double length, double square, double elasticModulus, double permissibleStress, double xLeftForce, double xRightForce, double xQForce)
{
    this->length = length;
    this->square = square;
    this->elasticModulus = elasticModulus;
    this->permissibleStress = permissibleStress;
    this->xLeftForce = xLeftForce;
    this->xRightForce = xRightForce;
    this->xQForce = xQForce;
}

SaprElement::SaprElement(double length, double square, double elasticModulus, double permissibleStress, double xLeftForce, double xRightForce, double xQForce, bool hasLeftSup, bool hasRightSup)
{
    this->length = length;
    this->square = square;
    this->elasticModulus = elasticModulus;
    this->permissibleStress = permissibleStress;
    this->xLeftForce = xLeftForce;
    this->xRightForce = xRightForce;
    this->xQForce = xQForce;
    this->hasLeftSupport = hasLeftSup;
    this->hasRightSupport = hasRightSup;
}

void SaprElement::draw(QPainter &painter, double maxHeight)
{
    if (isSelected)
    {
        painter.setPen(Qt::cyan);
    }

    painter.drawRect(QRect(x, y, width, height));
    if (maxHeight > 0) drawParams(painter, maxHeight);

    if (isSelected) painter.setPen(QPen(Qt::cyan, 3));
    else painter.setPen(QPen(Qt::black, 3));
    if (xLeftForce != 0)
    {
        if (xLeftForce > 0)
        {
            drawArrow(painter, x, y + height / 2, x + width * forceArrowIndentCoeff, y + height / 2);
            painter.drawText(QPoint(x + width * forceArrowIndentCoeff / 2, y + height / 2 - yQForceArrowIndent), QString::number(xLeftForce) + "F");
        }
        else if (xLeftForce < 0 && leftConnectedElement == nullptr)
        {
            drawArrow(painter, x, y + height / 2, x - width * forceArrowIndentCoeff, y + height / 2);
            painter.drawText(QPoint(x - width * forceArrowIndentCoeff / 2, y + height / 2 - yQForceArrowIndent), QString::number(xLeftForce) + "F");
        }
    }
    if (xRightForce != 0)
    {
        if (xRightForce > 0 && rightConnectedElement == nullptr)
        {
            drawArrow(painter, x + width, y + height / 2, x + width * (1.0 + forceArrowIndentCoeff), y + height / 2);
            painter.drawText(QPoint(x + width + width * forceArrowIndentCoeff / 2, y + height / 2 - yQForceArrowIndent), QString::number(xRightForce) + "F");
        }
        else if (xRightForce < 0)
        {
            drawArrow(painter, x + width, y + height / 2, x + width * (1.0 - forceArrowIndentCoeff), y + height / 2);
            painter.drawText(QPoint(x + width - width * forceArrowIndentCoeff / 2, y + height / 2 - yQForceArrowIndent), QString::number(xRightForce) + "F");
        }
    }
    if (isSelected) painter.setPen(QPen(Qt::cyan, 1));
    else painter.setPen(QPen(Qt::black, 1));
    if(xQForce != 0)
    {
        painter.drawLine(x, y + height / 2, x + width, y + height / 2);
        if (xQForce > 0)
        {
            for (int i = 0; i < (width / qForceArrowIndent) - 1; i++)
            {
                drawArrow(painter, x + qForceArrowIndent * i, y + height / 2, x + qForceArrowIndent * (i + 1), y + height / 2);
            }
        }
        else
        {
            for (int i = 0; i < (width / qForceArrowIndent) - 1; i++)
            {
                drawArrow(painter, x + width - qForceArrowIndent * i, y + height / 2, x + width - qForceArrowIndent * (i + 1), y + height / 2);
            }
        }
        painter.drawText(QPoint(x + width / 2 - 3, y + height / 2 - yQForceArrowIndent), QString::number(xQForce) + "q");
    }

    if (isSelected) painter.setPen(Qt::black);
}

void SaprElement::drawArrow(QPainter &painter, double stX, double stY, double endX, double endY)
{
    painter.drawLine(stX, stY, endX, endY);
    bool direct = false;
    if (stX > endX) direct = true;
    drawArrowHead(painter, endX, endY, direct);
}

void SaprElement::drawArrowHead(QPainter &painter, double x, double y, bool isDirectLeft)
{
    int mult = isDirectLeft ? 1 : -1;
    painter.drawLine(x + xQForceArrowIndent * mult, y + yQForceArrowIndent, x, y);
    painter.drawLine(x + xQForceArrowIndent * mult, y - yQForceArrowIndent, x, y);
}

void SaprElement::drawParams(QPainter &painter, double maxHeight)
{
    painter.drawLine(x, y + height / 2, x, y + height / 2 + maxHeight * paramIndentMultiply);
    painter.drawLine(x + width, y + height / 2, x + width, y + height / 2 + maxHeight * paramIndentMultiply);
    drawArrowHead(painter, x, y + height / 2 + maxHeight * paramIndentMultiply, true);
    drawArrowHead(painter, x + width, y + height / 2 + maxHeight * paramIndentMultiply, false);
    painter.drawLine(x, y + height / 2 + maxHeight * paramIndentMultiply, x + width, y + height / 2 + maxHeight * paramIndentMultiply);
    painter.drawText(QPoint(x + width / 2 - 10, y + height / 2 + maxHeight * paramIndentMultiply + 15), QString::number(length) + "L"); // DRAW TEXT PARAMS
    painter.drawText(QPoint(x + width / 4, y - 10), QString::number(elasticModulus) + "E   " + QString::number(square) + "A");
}

void SaprElement::drawDiagram(QPainter &painter, std::vector<double*> points, int size, double maxHeight, double** borderVals)
{
    double nMinVal = borderVals[0][0], nPointDelt = borderVals[0][1] - borderVals[0][0];
    double uMinVal = borderVals[1][0], uPointDelt = borderVals[1][1] - borderVals[1][0];
    double sMinVal = borderVals[1][0], sPointDelt = borderVals[2][1] - borderVals[2][0];
    if (nPointDelt == 0) nPointDelt = 1;
    if (uPointDelt == 0) uPointDelt = 1;
    if (sPointDelt == 0) sPointDelt = 1;

    painter.drawLine(x, y + height / 2, x, y + height / 2 + maxHeight * (paramIndentMultiply + 3 * (diagramIndentMultiply + diagramSizeMultiply)));
    painter.drawLine(x + width, y + height / 2, x + width, y + height / 2 + maxHeight * (paramIndentMultiply + 3 * (diagramIndentMultiply + diagramSizeMultiply)));

    // DRAW ZERO LINES
    painter.drawLine(x, y + height / 2 + maxHeight * (paramIndentMultiply + diagramIndentMultiply + diagramSizeMultiply) - (0 - nMinVal) / nPointDelt * maxHeight * diagramSizeMultiply,
                     x + width, y + height / 2 + maxHeight * (paramIndentMultiply + diagramIndentMultiply + diagramSizeMultiply) - (0 - nMinVal) / nPointDelt * maxHeight * diagramSizeMultiply);
    painter.drawLine(x, y + height / 2 + maxHeight * (paramIndentMultiply + 2 * diagramIndentMultiply + 2 * diagramSizeMultiply) - (0 - uMinVal) / uPointDelt * maxHeight * diagramSizeMultiply,
                     x + width, y + height / 2 + maxHeight * (paramIndentMultiply + 2 * diagramIndentMultiply + 2 * diagramSizeMultiply) - (0 - uMinVal) / uPointDelt * maxHeight * diagramSizeMultiply);
    painter.drawLine(x, y + height / 2 + maxHeight * (paramIndentMultiply + 3 * (diagramIndentMultiply + diagramSizeMultiply)) - (0 - sMinVal) / sPointDelt * maxHeight * diagramSizeMultiply,
                     x + width, y + height / 2 + maxHeight * (paramIndentMultiply + 3 * (diagramIndentMultiply + diagramSizeMultiply)) - (0 - sMinVal) / sPointDelt * maxHeight * diagramSizeMultiply);

    // DRAW GRAPH LABELS
    if (!leftConnectedElement)
    {
        painter.drawText(QPoint(x - 25, y + height / 2 + maxHeight * (paramIndentMultiply + diagramIndentMultiply + diagramSizeMultiply) -
                                            (0 - nMinVal) / nPointDelt * maxHeight * diagramSizeMultiply), QString("Nx"));
        painter.drawText(QPoint(x - 25, y + height / 2 + maxHeight * (paramIndentMultiply + 2 * diagramIndentMultiply + 2 * diagramSizeMultiply) -
                                            (0 - uMinVal) / uPointDelt * maxHeight * diagramSizeMultiply), QString("Ux"));
        painter.drawText(QPoint(x - 25, y + height / 2 + maxHeight * (paramIndentMultiply + 3 * (diagramIndentMultiply + diagramSizeMultiply)) -
                                            (0 - sMinVal) / sPointDelt * maxHeight * diagramSizeMultiply), QString("Sx"));
    }

//    painter.setPen(Qt::green); // Draw diagrams border
//    painter.drawRect(x, y + height / 2 + maxHeight * (paramIndentMultiply + diagramIndentMultiply), width, maxHeight * diagramSizeMultiply);
//    painter.setPen(Qt::yellow);
//    painter.drawRect(x, y + height / 2 + maxHeight * (paramIndentMultiply + 2 * diagramIndentMultiply + diagramSizeMultiply), width, maxHeight * diagramSizeMultiply);
//    painter.setPen(Qt::black);

    for (int i = 0; i <= size; i++)
    {
        painter.drawPoint(x + width / size * i, y + height / 2 + maxHeight * (paramIndentMultiply + diagramIndentMultiply + diagramSizeMultiply) -
                                                    (points[0][i] - nMinVal) / nPointDelt * maxHeight * diagramSizeMultiply);
        painter.drawPoint(x + width / size * i, y + height / 2 + maxHeight * (paramIndentMultiply + 2 * (diagramIndentMultiply + diagramSizeMultiply)) -
                                                    (points[1][i] - uMinVal) / uPointDelt * maxHeight * diagramSizeMultiply);
        painter.drawPoint(x + width / size * i, y + height / 2 + maxHeight * (paramIndentMultiply + 3 * (diagramIndentMultiply + diagramSizeMultiply)) -
                                                    (points[2][i] - sMinVal) / sPointDelt * maxHeight * diagramSizeMultiply);

        if (i % 60 == 0)
        {
            painter.drawLine(x + width / size * i, y + height / 2 + maxHeight * (paramIndentMultiply + diagramIndentMultiply + diagramSizeMultiply) -
                                                       (points[0][i] - nMinVal) / nPointDelt * maxHeight * diagramSizeMultiply,
                             x + width / size * i, y + height / 2 + maxHeight * (paramIndentMultiply + diagramIndentMultiply + diagramSizeMultiply) -
                                                        (0 - nMinVal) / nPointDelt * maxHeight * diagramSizeMultiply);

            painter.drawLine(x + width / size * i, y + height / 2 + maxHeight * (paramIndentMultiply + 2 * (diagramIndentMultiply + diagramSizeMultiply)) -
                                                       (points[1][i] - uMinVal) / uPointDelt * maxHeight * diagramSizeMultiply,
                             x + width / size * i, y + height / 2 + maxHeight * (paramIndentMultiply + 2 * diagramIndentMultiply + 2 * diagramSizeMultiply) -
                                                       (0 - uMinVal) / uPointDelt * maxHeight * diagramSizeMultiply);

            painter.drawLine(x + width / size * i, y + height / 2 + maxHeight * (paramIndentMultiply + 3 * (diagramIndentMultiply + diagramSizeMultiply)) -
                                                       (points[2][i] - sMinVal) / sPointDelt * maxHeight * diagramSizeMultiply,
                             x + width / size * i, y + height / 2 + maxHeight * (paramIndentMultiply + 3 * (diagramIndentMultiply + diagramSizeMultiply)) -
                                 (0 - sMinVal) / sPointDelt * maxHeight * diagramSizeMultiply);
        }
    }

    painter.setPen(Qt::red);
    QFont font;
    QFontMetrics fM(font);

    double yIndentMult = points[0][0] < 0 ? 2.2 : -1;
    painter.drawText(QPoint(x + 2, y + height / 2 + (6 * yIndentMult) + maxHeight * (paramIndentMultiply + diagramIndentMultiply + diagramSizeMultiply) -
                                       (points[0][0] - nMinVal) / nPointDelt * maxHeight * diagramSizeMultiply), doubleToQString(points[0][0]));
    int indent = fM.horizontalAdvance(doubleToQString(points[0][size]));
    yIndentMult = points[0][size] < 0 ? 2.2 : -1;
    painter.drawText(QPoint(x + width - 2 - indent, y + height / 2 + (6 * yIndentMult) + maxHeight * (paramIndentMultiply + diagramIndentMultiply + diagramSizeMultiply) -
                                                (points[0][size] - nMinVal) / nPointDelt * maxHeight * diagramSizeMultiply), doubleToQString(points[0][size]));

    yIndentMult = points[1][0] < 0 ? 2.2 : -1;
    painter.drawText(QPoint(x + 2, y + height / 2 + (6 * yIndentMult) + maxHeight * (paramIndentMultiply + 2 * diagramIndentMultiply + 2 * diagramSizeMultiply) -
                                       (points[1][0] - uMinVal) / uPointDelt * maxHeight * diagramSizeMultiply), doubleToQString(points[1][0]));
    indent = fM.horizontalAdvance(doubleToQString(points[1][size]));
    yIndentMult = points[1][size] < 0 ? 2.2 : -1;
    painter.drawText(QPoint(x + width - 2 - indent, y + height / 2 + (6 * yIndentMult) + maxHeight * (paramIndentMultiply + 2 * diagramIndentMultiply + 2 * diagramSizeMultiply) -
                                                (points[1][size] - uMinVal) / uPointDelt * maxHeight * diagramSizeMultiply), doubleToQString(points[1][size]));

    yIndentMult = points[2][0] < 0 ? 2.2 : -1;
    painter.drawText(QPoint(x + 2, y + height / 2 + (6 * yIndentMult) + maxHeight * (paramIndentMultiply + 3 * (diagramIndentMultiply + diagramSizeMultiply)) -
                                       (points[2][0] - sMinVal) / sPointDelt * maxHeight * diagramSizeMultiply), doubleToQString(points[2][0]));
    indent = fM.horizontalAdvance(doubleToQString(points[2][size]));
    yIndentMult = points[2][size] < 0 ? 2.2 : -1;
    painter.drawText(QPoint(x + width - 2 - indent, y + height / 2 + (6 * yIndentMult) + maxHeight * (paramIndentMultiply + 3 * (diagramIndentMultiply + diagramSizeMultiply)) -
                                                        (points[2][size] - sMinVal) / sPointDelt * maxHeight * diagramSizeMultiply), doubleToQString(points[2][size]));

    painter.setPen(Qt::black);
}

void SaprElement::zoom(double multiply)
{
    width *= multiply;
    height *= multiply;
    sizeMultiply *= multiply;
}

void SaprElement::setLeftForce(double force)
{
    xLeftForce = force;
    if (leftConnectedElement != nullptr) leftConnectedElement->xRightForce = force;
}

void SaprElement::setRightForce(double force)
{
    xRightForce = force;
    if (rightConnectedElement != nullptr) rightConnectedElement->xLeftForce = force;
}

void SaprElement::setLeftConnection(SaprElement *element)
{
    if (leftConnectedElement != nullptr) leftConnectedElement->rightConnectedElement = nullptr;
    if (element != nullptr) element->rightConnectedElement = this;
    leftConnectedElement = element;
}

void SaprElement::setRightConnection(SaprElement *element)
{
    if (rightConnectedElement != nullptr) rightConnectedElement->leftConnectedElement = nullptr;
    if (element != nullptr) element->leftConnectedElement = this;
    rightConnectedElement = element;
}

QString SaprElement::doubleToQString(double num)
{
    char format = 'f';
    int precision = 2;

    if (abs(num) > 1000)
    {
        std::string str = std::to_string(num);
        if (str[1] == '0' && str[2] == '0') precision = 0;
        format = 'e';
    }
    if (int(num * 100) % 100 == 0) precision = 0;

    return QString::number(num, format, precision);
}
