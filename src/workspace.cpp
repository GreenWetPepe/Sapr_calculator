#include "workspace.h"
#include "qdebug.h"
#include "Options.cpp"

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
    double maxHeight = getMaxHeight();

    for(SaprElement *el : elements)
    {
        el->draw(painter, maxHeight);
    }

    drawSupports(painter, maxHeight);
}

void WorkSpace::drawSupports(QPainter &painter, int maxHeight)
{
    for (auto el : elements)
    {
        el->drawSupports(painter, maxHeight);
    }
}

void WorkSpace::drawDiagram(QPainter &painter)
{
    if (calculation.empty()) return;
    SaprElement *firstElement = getFirstLinkedElement();
    int maxHeight = getMaxHeight();

    double minNx = 0, maxNx = 0;
    double minUx = 0, maxUx = 0;
    double minSx = 0, maxSx = 0;

    for (auto calc : calculation)
    {
        for (int i = 0; i < calc[0].size(); i++)
        {
            if (minNx > calc[0][i]) minNx = calc[0][i];
            if (maxNx < calc[0][i]) maxNx = calc[0][i];
            if (minUx > calc[1][i]) minUx = calc[1][i];
            if (maxUx < calc[1][i]) maxUx = calc[1][i];
            if (minSx > calc[2][i]) minSx = calc[2][i];
            if (maxSx < calc[2][i]) maxSx = calc[2][i];
        }
    }

    SaprElement *el = firstElement;
    int i = 0;
    while (el != nullptr)
    {

        el->drawDiagram(painter, calculation[i][0], maxHeight, minNx, maxNx, options::diagram::nXIndent, "Nx");
        el->drawDiagram(painter, calculation[i][1], maxHeight, minUx, maxUx, options::diagram::uXIndent, "Ux");
        el->drawDiagram(painter, calculation[i][2], maxHeight, minSx, maxSx, options::diagram::sXIndent, "Sx");
        el = el->getRightConnectedElement();
        i++;
    }
}

SaprElement* WorkSpace::findElement(int x, int y)
{
    for (auto el : elements)
    {
        if (x >= el->getX() && y >= el->getY() && x <= el->getX() + el->getWidth() && y <= el->getY() + el->getHeight())
        {
            return el;
        }
    }

    return nullptr;
}

SaprElement* WorkSpace::getFirstLinkedElement()
{
    for (auto el : elements)
    {
        if (!el->getLeftConnectedElement())
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
            if (elements[i]->getLeftConnectedElement() != nullptr) elements[i]->getLeftConnectedElement()->setRightConnection(nullptr);
            if (elements[i]->getRightConnectedElement() != nullptr) elements[i]->getRightConnectedElement()->setLeftConnection(nullptr);
            delete elements[i];
            elements.erase(elements.begin() + i);
        }
    }
    autoSizeElements();
    dropCalc();
}

int WorkSpace::getMaxHeight()
{
    int maxHeight = 0;
    for (auto el : elements)
    {
        if (maxHeight < el->getHeight()) maxHeight = el->getHeight();
    }
    return maxHeight;
}

void WorkSpace::checkForConnections(SaprElement *element)
{
    element->setLeftConnection(nullptr);
    element->setRightConnection(nullptr);

    for (auto el : elements)
    {
        if (element != el)
        {
            int connectionType = element->checkForConnection(el);
            if (connectionType == options::saprElement::leftConnection)
            {
                el->setRightConnection(element);
                el->correctPosToLinkedElement();
            }
            else if (connectionType == options::saprElement::rightConnection)
            {
                el->setLeftConnection(element);
                element->correctPosToLinkedElement();
            }
        }
    }
    dropCalc();
}

bool WorkSpace::checkSystemReadiness()
{
    int supportsCount = 0, linkedElementsCount = 0;

    SaprElement *firstElement = getFirstLinkedElement();
    while(firstElement)
    {
        if (firstElement->hasLeftSupport()) supportsCount++;
        if (firstElement->hasRightSupport()) supportsCount++;
        linkedElementsCount++;
        firstElement = firstElement->getRightConnectedElement();
    }

    if (supportsCount == 0 || supportsCount > 2 || linkedElementsCount != elements.size()) return false;
    return true;
}

void WorkSpace::calc()
{
    if (!checkSystemReadiness()) return;
    CalculationProducer::dropCalculation();
    CalculationProducer::calculateArguments(elements);
    calculation = CalculationProducer::calcResults(getFirstLinkedElement());
}

void WorkSpace::dropCalc()
{
    calculation.clear();
}

void WorkSpace::autoSizeElements()
{
    double maxLength = -1, maxSquare = -1;
    for (auto el : elements)
    {
        if (maxLength < el->getLength()) maxLength = el->getLength();
        if (maxSquare < el->getSquare()) maxSquare = el->getSquare();
    }

    for (auto el : elements)
    {
        el->adjustSize(maxLength, maxSquare, windowWidth, windowHeight, sizeMult);
    }

    correctLinkedElementsPos();
}

void WorkSpace::correctLinkedElementsPos()
{
    for (auto el : elements)
    {
        if (!el->getLeftConnectedElement())
        {
            SaprElement *linkedElement = el;
            while (linkedElement->getRightConnectedElement())
            {
                linkedElement->correctPosToLinkedElement();
                linkedElement = linkedElement->getRightConnectedElement();
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
        el->move(deltX, deltY);
    }
}

void WorkSpace::setWindowSize(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
}

