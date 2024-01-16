#include "saprelement.h"
#include "Options.cpp"

#include <QDebug>

SaprElement::SaprElement()
{

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
    selected = el.selected;
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
    this->leftSupport = hasLeftSup;
    this->rightSupport = hasRightSup;
}

void SaprElement::draw(QPainter &painter, double maxHeight)
{
    if (selected) painter.setPen(Qt::cyan);

    painter.drawRect(QRect(x, y, width, height));
    if (maxHeight > 0) drawParams(painter, maxHeight);

    drawForces(painter);
    drawDisturbedForces(painter);

    painter.setPen(Qt::black);
}

void SaprElement::drawForces(QPainter &painter)
{
    if (xLeftForce != 0)
    {
        if (xLeftForce > 0)
        {
            drawArrow(painter, x, y + height / 2, x + width * options::saprElement::forceArrowIndentCoeff, y + height / 2);
            painter.drawText(QPoint(x + width * options::saprElement::forceArrowIndentCoeff / 2, y + height / 2 -
                                                options::saprElement::yQForceArrowIndent), QString::number(abs(xLeftForce)) + "F");
        }
        else if (xLeftForce < 0 && leftConnectedElement == nullptr)
        {
            drawArrow(painter, x, y + height / 2, x - width * options::saprElement::forceArrowIndentCoeff, y + height / 2);
            painter.drawText(QPoint(x - width * options::saprElement::forceArrowIndentCoeff / 2, y + height / 2 -
                                                options::saprElement::yQForceArrowIndent), QString::number(abs(xLeftForce)) + "F");
        }
    }
    if (xRightForce != 0)
    {
        if (xRightForce > 0 && rightConnectedElement == nullptr)
        {
            drawArrow(painter, x + width, y + height / 2, x + width * (1.0 + options::saprElement::forceArrowIndentCoeff), y + height / 2);
            painter.drawText(QPoint(x + width + width * options::saprElement::forceArrowIndentCoeff / 2, y + height / 2 -
                                                        options::saprElement::yQForceArrowIndent), QString::number(abs(xRightForce)) + "F");
        }
        else if (xRightForce < 0)
        {
            drawArrow(painter, x + width, y + height / 2, x + width * (1.0 - options::saprElement::forceArrowIndentCoeff), y + height / 2);
            painter.drawText(QPoint(x + width - width * options::saprElement::forceArrowIndentCoeff / 2, y + height / 2 -
                                                        options::saprElement::yQForceArrowIndent), QString::number(abs(xRightForce)) + "F");
        }
    }
}

void SaprElement::drawDisturbedForces(QPainter &painter)
{
    if(xQForce != 0)
    {
        painter.drawLine(x, y + height / 2, x + width, y + height / 2);
        if (xQForce > 0)
        {
            for (int i = 0; i < (width / options::saprElement::qForceArrowIndent) - 1; i++)
            {
                drawArrow(painter, x + options::saprElement::qForceArrowIndent * i, y + height / 2, x +
                                                                    options::saprElement::qForceArrowIndent * (i + 1), y + height / 2);
            }
        }
        else
        {
            for (int i = 0; i < (width / options::saprElement::qForceArrowIndent) - 1; i++)
            {
                drawArrow(painter, x + width - options::saprElement::qForceArrowIndent * i, y + height / 2, x + width -
                                                options::saprElement::qForceArrowIndent * (i + 1), y + height / 2);
            }
        }
        painter.drawText(QPoint(x + width / 2 - 3, y + height / 2 - options::saprElement::yQForceArrowIndent),
                         QString::number(abs(xQForce)) + "q");
    }
}

void SaprElement::drawArrow(QPainter &painter, int stX, int stY, int endX, int endY)
{
    painter.drawLine(stX, stY, endX, endY);
    bool direct = false;
    if (stX > endX) direct = true;
    drawArrowHead(painter, endX, endY, direct);
}

void SaprElement::drawArrowHead(QPainter &painter, int x, int y, bool isDirectLeft)
{
    int mult = isDirectLeft ? 1 : -1;
    painter.drawLine(x + options::saprElement::xQForceArrowIndent * mult, y + options::saprElement::yQForceArrowIndent, x, y);
    painter.drawLine(x + options::saprElement::xQForceArrowIndent * mult, y - options::saprElement::yQForceArrowIndent, x, y);
}

void SaprElement::drawParams(QPainter &painter, int maxHeight)
{
    painter.drawLine(x, y + height / 2, x, y + height / 2 + maxHeight * options::saprElement::paramIndentMultiply);
    painter.drawLine(x + width, y + height / 2, x + width, y + height / 2 + maxHeight * options::saprElement::paramIndentMultiply);
    drawArrowHead(painter, x, y + height / 2 + maxHeight * options::saprElement::paramIndentMultiply, true);
    drawArrowHead(painter, x + width, y + height / 2 + maxHeight * options::saprElement::paramIndentMultiply, false);
    painter.drawLine(x, y + height / 2 + maxHeight * options::saprElement::paramIndentMultiply, x + width, y + height / 2 + maxHeight * options::saprElement::paramIndentMultiply);
    painter.drawText(QPoint(x + width / 2 - 10, y + height / 2 + maxHeight * options::saprElement::paramIndentMultiply + 15), QString::number(length) + "L"); // DRAW TEXT PARAMS
    painter.drawText(QPoint(x + width / 4, y - 10), QString::number(elasticModulus) + "E   " + QString::number(square) + "A");
}

void SaprElement::drawSupports(QPainter &painter, int maxHeight)
{
    int supportsLineCount = 23;
    if (leftSupport)
    {
        painter.drawLine(x - 3, y + height / 2 - (maxHeight + 100) / 2, x - 3, y + height / 2 + (maxHeight + 100) / 2);
        for (int i = 0; i < supportsLineCount; i++)
        {
            painter.drawLine(x - 6, y + height / 2 - (maxHeight + 100) / 2 + (maxHeight + 100) / supportsLineCount * i,
                             x - 15, y + height / 2 - (maxHeight + 100) / 2 + (maxHeight + 100) / supportsLineCount * i + 8);
        }
    }
    if (rightSupport)
    {
        painter.drawLine(x + width + 3, y + height / 2 - (maxHeight + 100) / 2, x + width + 3, y + height / 2 + (maxHeight + 100) / 2);
        for (int i = 0; i < supportsLineCount; i++)
        {
            painter.drawLine(x + width + 15, y + height / 2 - (maxHeight + 100) / 2 + (maxHeight + 100) / supportsLineCount * i,
                             x + width + 6, y + height / 2 - (maxHeight + 100) / 2 + (maxHeight + 100) / supportsLineCount * i + 8);
        }
    }
}

void SaprElement::drawDiagram(QPainter &painter, std::vector<double> points, int maxHeight, double minVal, double maxVal, int indent, std::string label)
{
    double pointsDelt = maxVal - minVal;
    if (pointsDelt == 0) pointsDelt = 1;

    painter.drawLine(x, y + height / 2 + maxHeight * options::saprElement::paramIndentMultiply,
                     x, y + height / 2 + maxHeight * indent);
    painter.drawLine(x + width, y + height / 2 + maxHeight * options::saprElement::paramIndentMultiply,
                     x + width, y + height / 2 + maxHeight * indent);

    // DRAW ZERO LINES
    painter.drawLine(x, y + height / 2 + maxHeight * indent - (0 - minVal) / pointsDelt * maxHeight * options::diagram::diagramSizeMultiply,
                     x + width, y + height / 2 + maxHeight * indent - (0 - minVal) / pointsDelt * maxHeight * options::diagram::diagramSizeMultiply);

    if (!leftConnectedElement)
    {
        painter.drawText(QPoint(x - 25, y + height / 2 + maxHeight * options::diagram::nXIndent -
                                            (0 - minVal) / (pointsDelt) *
                                                maxHeight * options::diagram::diagramSizeMultiply), QString::fromStdString(label));
    }

//    painter.setPen(Qt::green); // Draw diagrams border
//    painter.drawRect(x, y + height / 2 + maxHeight * indent, width, maxHeight * diagramSizeMultiply);
//    painter.setPen(Qt::black);

    for (int i = 0; i <= points.size(); i++)
    {
        painter.drawPoint(x + width / points.size() * i, y + height / 2 + maxHeight * indent - (points[i] - minVal) /
                                                                            pointsDelt * maxHeight * options::diagram::diagramSizeMultiply);

        if (i % 60 == 0)
        {
            painter.drawLine(x + width / points.size() * i, y + height / 2 + maxHeight * indent -
                                                       (points[i] - minVal) / pointsDelt * maxHeight * options::diagram::diagramSizeMultiply,
                             x + width / points.size() * i, y + height / 2 + maxHeight * indent -
                                                        (0 - minVal) / pointsDelt * maxHeight * options::diagram::diagramSizeMultiply);
        }
    }

    painter.setPen(Qt::red);
    QFont font;
    QFontMetrics fM(font);

    double yIndentMult = points[0] < 0 ? 2.2 : -1;
    painter.drawText(QPoint(x + 2, y + height / 2 + (6 * yIndentMult) + maxHeight * indent -
                                       (points[0] - minVal) / pointsDelt * maxHeight * options::diagram::diagramSizeMultiply), doubleToQString(points[0]));
    int textIndent = fM.horizontalAdvance(doubleToQString(points.back()));
    yIndentMult = points.back() < 0 ? 2.2 : -1;
    painter.drawText(QPoint(x + width - 2 - textIndent, y + height / 2 + (6 * yIndentMult) + maxHeight * indent -
                                        (points.back() - minVal) / pointsDelt * maxHeight * options::diagram::diagramSizeMultiply), doubleToQString(points.back()));

    painter.setPen(Qt::black);
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

void SaprElement::zoom(double multiply)
{
    width *= multiply;
    height *= multiply;
}

void SaprElement::adjustSize(double maxLength, double maxSquare, int windowWidth, int windowHeight, double sizeMult)
{
    width = windowWidth * options::workSpace::maxElementWidthRelationToWindow * (options::workSpace::minElementWidthRelationToMaxWidth +
                                                    length / (maxLength / options::workSpace::minElementWidthRelationToMaxWidth)) * sizeMult;
    height = windowHeight * options::workSpace::maxElementHeightRelationToWindow * (options::workSpace::minElementHeightRelationToMaxHeight +
                                                    square / (maxSquare / options::workSpace::minElementHeightRelationToMaxHeight)) * sizeMult;
}

void SaprElement::move(int x, int y)
{
    this->x += x;
    this->y += y;
}

void SaprElement::correctPosToLinkedElement()
{
    if (!rightConnectedElement) return;
    rightConnectedElement->x = x + width;
    rightConnectedElement->y = y + height / 2 - rightConnectedElement->height / 2;
}

int SaprElement::checkForConnection(SaprElement *element)
{
    if (x > element->x)
    {
        if (abs(x - (element->x + element->width)) <= options::workSpace::xElementConnectionSpread &&
            abs(y + height / 2 - (element->y + element->height / 2)) <= options::workSpace::yElementConnectionSpread)
        {
            return options::saprElement::leftConnection;
        }
        return options::saprElement::noConnection;
    }
    else
    {
        if (abs(element->x - (x + width)) <= options::workSpace::xElementConnectionSpread &&
            abs(y + height / 2 - (element->y + element->height / 2)) <= options::workSpace::yElementConnectionSpread)
        {
            return options::saprElement::rightConnection;
        }
        return options::saprElement::noConnection;
    }
}

void SaprElement::setData(SaprElementData data)
{
    setLength(data.length);
    setSquare(data.square);
    setElasticModulus(data.elasticModulus);
    setPermissibleStress(data.permissibleStress);
    setXQForce(data.xQForce);
    setLeftForce(data.xLeftForce);
    setRightForce(data.xRightForce);
    setLeftSupport(data.hasLeftSupport);
    setRightSupport(data.hasRightSupport);
}

SaprElementData SaprElement::getData()
{
    SaprElementData data;
    data.length = length;
    data.square = square;
    data.elasticModulus = elasticModulus;
    data.permissibleStress = permissibleStress;
    data.xLeftForce = xQForce;
    data.xRightForce = xLeftForce;
    data.xQForce = xRightForce;
    data.hasLeftSupport = leftSupport;
    data.hasRightSupport = rightSupport;
    return data;
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
    if (element != nullptr)
    {
        element->rightConnectedElement = this;
        element->setRightSupport(false);
        setLeftSupport(false);
        setLeftForce(xLeftForce);
    }
    leftConnectedElement = element;
}

void SaprElement::setRightConnection(SaprElement *element)
{
    if (rightConnectedElement != nullptr) rightConnectedElement->leftConnectedElement = nullptr;
    if (element != nullptr)
    {
        element->leftConnectedElement = this;
        element->setLeftSupport(false);
        setRightSupport(false);
        setRightForce(xRightForce);
    }
    rightConnectedElement = element;
}

void SaprElement::setLength(double length)
{
    if (length > 0) this->length = length;
}

void SaprElement::setSquare(double square)
{
    if (square > 0) this->square = square;
}

void SaprElement::setElasticModulus(double elasticModulus)
{
    if (elasticModulus > 0) this->elasticModulus = elasticModulus;
}

void SaprElement::setPermissibleStress(double permissibleStress)
{
    if (permissibleStress > 0) this->permissibleStress = permissibleStress;
}

void SaprElement::setXQForce(double xQForce)
{
    this->xQForce = xQForce;
}

void SaprElement::setLeftSupport(bool leftSupport)
{
    if (leftSupport)
    {
        if (!leftConnectedElement) this->leftSupport = true;
    }
    else
    {
        this->leftSupport = false;
    }
}

void SaprElement::setRightSupport(bool rightSupport)
{
    if (rightSupport)
    {
        if (!rightConnectedElement) this->rightSupport = true;
    }
    else
    {
        this->rightSupport = false;
    }
}

void SaprElement::setSelected(bool selected)
{
    this->selected = selected;
}

int SaprElement::getX()
{
    return x;
}

int SaprElement::getY()
{
    return y;
}

int SaprElement::getWidth()
{
    return width;
}

int SaprElement::getHeight()
{
    return height;
}

double SaprElement::getLength()
{
    return length;
}

double SaprElement::getSquare()
{
    return square;
}

double SaprElement::getElasticModulus()
{
    return elasticModulus;
}

double SaprElement::getPermissibleStress()
{
    return permissibleStress;
}

double SaprElement::getXLeftForce()
{
    return xLeftForce;
}

double SaprElement::getXRightForce()
{
    return xRightForce;
}

double SaprElement::getXQForce()
{
    return xQForce;
}

bool SaprElement::hasLeftSupport()
{
    return leftSupport;
}

bool SaprElement::hasRightSupport()
{
    return rightSupport;
}

bool SaprElement::isSelected()
{
    return selected;
}

SaprElement* SaprElement::getLeftConnectedElement()
{
    return leftConnectedElement;
}

SaprElement* SaprElement::getRightConnectedElement()
{
    return rightConnectedElement;
}
