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
    if (!CalculationProducer::isReady()) return;
    SaprElement *firstElement = getFirstLinkedElement();
    int maxHeight = getMaxHeight();

    std::vector<std::vector<std::vector<double>>> res = CalculationProducer::calcResults(firstElement);

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
            for (int j = 0; j < options::diagram::pointsCount; j++)
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

        el->drawDiagram(painter, res[i][0], maxHeight, borderVals[0][0], borderVals[0][1], options::diagram::nXIndent, "Nx");
        el->drawDiagram(painter, res[i][1], maxHeight, borderVals[1][0], borderVals[1][1], options::diagram::uXIndent, "Ux");
        el->drawDiagram(painter, res[i][2], maxHeight, borderVals[2][0], borderVals[2][1], options::diagram::sXIndent, "Sx");
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
        if (!el->getRightConnectedElement())
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

