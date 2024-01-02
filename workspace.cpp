#include "workspace.h"
#include "qdebug.h"

#include <fstream>
#include <cmath>
#include <sstream>


WorkSpace::WorkSpace()
{
}

WorkSpace::WorkSpace(std::string path)
{
    std::ifstream f(path);
    if (!f.is_open())
    {
        std::cerr << "CAN'T OPEN FILE";
        return;
    }

    std::string str;
    while (std::getline(f, str))
    {
//        SaprElement sEl(str);
//        addElement(sEl);
    }
}

void WorkSpace::drawElements(QPainter &painter)
{
    double maxHeight = -1;
    for (SaprElement *el : elements)
    {
        if (el->height > maxHeight) maxHeight = el->height;
    }

    for(SaprElement *el : elements)
    {
        el->draw(painter, maxHeight);
    }

    drawSupports(painter, maxHeight);
}

void WorkSpace::drawSupports(QPainter &painter, int maxHeight)
{
    int supportsLineCount = 23;
    for (auto el : elements)
    {
        if (el->hasLeftSupport)
        {
            painter.drawLine(el->x - 3, el->y + el->height / 2 - (maxHeight + 100) / 2, el->x - 3, el->y + el->height / 2 + (maxHeight + 100) / 2);
            for (int i = 0; i < supportsLineCount; i++)
            {
                painter.drawLine(el->x - 6, el->y + el->height / 2 - (maxHeight + 100) / 2 + (maxHeight + 100) / supportsLineCount * i,
                                 el->x - 15, el->y + el->height / 2 - (maxHeight + 100) / 2 + (maxHeight + 100) / supportsLineCount * i + 8);
            }
        }
        if (el->hasRightSupport)
        {
            painter.drawLine(el->x + el->width + 3, el->y + el->height / 2 - (maxHeight + 100) / 2, el->x + el->width + 3, el->y + el->height / 2 + (maxHeight + 100) / 2);
            for (int i = 0; i < supportsLineCount; i++)
            {
                painter.drawLine(el->x + el->width + 15, el->y + el->height / 2 - (maxHeight + 100) / 2 + (maxHeight + 100) / supportsLineCount * i,
                                 el->x + el->width + 6, el->y + el->height / 2 - (maxHeight + 100) / 2 + (maxHeight + 100) / supportsLineCount * i + 8);
            }
        }
    }
}

void WorkSpace::drawDiagram(QPainter &painter)
{
    if (!CalculationProducer::isReady()) return;
    int resSize = 1500;
    double maxHeight = 0;
    SaprElement *firstElement = nullptr;
    for (auto el : elements)
    {
        if (el->leftConnectedElement == nullptr)
        {
            firstElement = el;
        }
        if (maxHeight < el->height) maxHeight = el->height;
    }

    std::vector<std::vector<double*>> res = CalculationProducer::calcResults(firstElement, resSize);

    double** borderVals = new double*[3];
    borderVals[0] = new double[2];
    borderVals[1] = new double[2];
    borderVals[2] = new double[2];
    borderVals[0][0] = 0, borderVals[0][1] = 0;
    borderVals[1][0] = 0, borderVals[1][1] = 0;
    borderVals[2][0] = 0, borderVals[2][1] = 0;
    for (auto calc : res)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < resSize; j++)
            {
                if (borderVals[i][0] > calc[i][j]) borderVals[i][0] = calc[i][j];
                if (borderVals[i][1] < calc[i][j]) borderVals[i][1] = calc[i][j];
            }
        }
    }

    SaprElement *el = firstElement;
    int i = 0;
    while (el != nullptr)
    {
        el->drawDiagram(painter, res[i], resSize, maxHeight, borderVals);
        el = el->rightConnectedElement;
        i++;
    }
}

SaprElement* WorkSpace::findElement(int x, int y)
{
    for(SaprElement *el : elements)
    {
        if (x >= el->x && y >= el->y && x <= el->x + el->width && y <= el->y + el->height)
        {
            return el;
        }
    }

    return nullptr;
}

void WorkSpace::addElement(SaprElement *element)
{
    elements.push_back(element);
    dropCalc();
}

void WorkSpace::removeElement(SaprElement *element)
{
    for (int i = 0; i < elements.size(); i++)
    {
        if (element == elements[i])
        {
            if (elements[i]->leftConnectedElement != nullptr) elements[i]->leftConnectedElement->rightConnectedElement = nullptr;
            if (elements[i]->rightConnectedElement != nullptr) elements[i]->rightConnectedElement->leftConnectedElement = nullptr;
            elements.erase(elements.begin() + i);
        }
    }
    autoSizeElements();
    dropCalc();
}

void WorkSpace::checkForConnection(SaprElement *element)
{
    element->setLeftConnection(nullptr);
    element->setRightConnection(nullptr);

    for(SaprElement *el : elements)
    {
        if (&element != &el)
        {
            if (element->x > el->x)
            {
                if (abs(element->x - (el->x + el->width)) <= xElementConnectionSpread && abs(element->y + element->height / 2 - (el->y + el->height / 2)) <= yElementConnectionSpread)
                {
                    element->x = el->x + el->width;
                    element->y = el->y - (element->height / 2 - el->height / 2);
                    element->setLeftConnection(el);
                    element->xLeftForce = element->leftConnectedElement->xRightForce;
                    element->hasLeftSupport = false;
                    el->hasRightSupport = false;
                }
            }
            else
            {
                if (abs(el->x - (element->x + element->width)) <= xElementConnectionSpread && abs(element->y + element->height / 2 - (el->y + el->height / 2)) <= yElementConnectionSpread)
                {
                    element->x = el->x - element->width + 1;
                    element->y = el->y - (element->height / 2 - el->height / 2);
                    element->setRightConnection(el);
                    element->xRightForce = element->rightConnectedElement->xLeftForce;
                    element->hasRightSupport = false;
                    el->hasLeftSupport = false;
                }
            }
        }
    }
    dropCalc();
}

bool WorkSpace::checkSystemReadiness()
{
    int supportsCount = 0, linkedElementsCount = 0;

    SaprElement *firstElement = getFirstSystemElement();
    while(firstElement)
    {
        if (firstElement->hasLeftSupport) supportsCount++;
        if (firstElement->hasRightSupport) supportsCount++;
        linkedElementsCount++;
        firstElement = firstElement->rightConnectedElement;
    }

    if (supportsCount == 0 || supportsCount > 2 || linkedElementsCount != elements.size()) return false;
    return true;
}

SaprElement* WorkSpace::getFirstSystemElement()
{
    SaprElement *firstElement;

    for (auto el : elements)
    {
        if (!el->leftConnectedElement) return el;
    }
}

void WorkSpace::calc()
{
    if (!checkSystemReadiness()) return;
    CalculationProducer::calculateArguments(elements);
}

void WorkSpace::dropCalc()
{
    CalculationProducer::dropCalculation();
}

void WorkSpace::autoSizeElements()
{
    double maxLength = -1, maxSquare = -1;
    for (auto el : elements)
    {
        if (maxLength < el->length) maxLength = el->length;
        if (maxSquare < el->square) maxSquare = el->square;
    }

    for (auto el : elements)
    {
        el->width = windowWidth * maxElementWidthRelationToWindow * (minElementWidthRalationToMaxWidth + el->length / (maxLength / minElementWidthRalationToMaxWidth)) * sizeMult;
        el->height = windowHeight * maxElementHeightRelationToWindow * (minElementHeightRalationToMaxHeight + el->square / (maxSquare / minElementHeightRalationToMaxHeight)) * sizeMult;
    }

    correctLinkedElementsPos();
}

void WorkSpace::correctLinkedElementsPos()
{
    for (auto el : elements)
    {
        if (el->leftConnectedElement == nullptr)
        {
            SaprElement *linkedElement = el;
            while (linkedElement->rightConnectedElement != nullptr)
            {
                linkedElement->rightConnectedElement->x = linkedElement->x + linkedElement->width;
                linkedElement->rightConnectedElement->y = linkedElement->y + linkedElement->height / 2 - linkedElement->rightConnectedElement->height / 2;
                linkedElement = linkedElement->rightConnectedElement;
            }
        }
    }
}

void WorkSpace::zoomIn()
{
    sizeMult *= 1.1;
    for(SaprElement *el : elements)
    {
        el->zoom(1.1);
    }
    correctLinkedElementsPos();
}

void WorkSpace::zoomOut()
{
    if (sizeMult < minSizeMult) return;
    sizeMult /= 1.1;
    for(SaprElement *el : elements)
    {
        el->zoom(1.0 / 1.1);
    }
    correctLinkedElementsPos();
}

void WorkSpace::moveElements(int deltX, int deltY)
{
    for(SaprElement *el : elements)
    {
        el->x += deltX;
        el->y += deltY;
    }
}

void WorkSpace::setWindowSize(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
}

