#include "filehandler.h"

#include <fstream>
#include <sstream>
#include <QDebug>

std::vector<SaprElement*> FileHandler::readElements;

void FileHandler::createFile(std::string path)
{
    std::ofstream fo;
    fo.open(path);
    fo << "";
    fo.close();
}

bool FileHandler::openProject(std::string path)
{
    std::ifstream f;
    std::ofstream fo;
    f.open(path);

    readElements.clear();
    try
    {
        std::string line;
        std::vector<int*> tempConnections;

        int i = 0;
        while (std::getline(f, line))
        {
            std::string item;
            std::stringstream sst(line);
            std::vector<double> nums;

            while (std::getline(sst, item, *" "))
            {
                nums.push_back(std::stod(item));
            }

            if (nums.size() != 10)
            {
                qDebug() << "Not enough variables";
                return false;
            }

            readElements.push_back(new SaprElement(nums[0], nums[1], nums[2], nums[3], nums[4], nums[5], nums[6], bool(nums[7]), bool(nums[8])));
            int *connection = new int[2];
            connection[0] = i;
            connection[1] = int(nums[9]);
            tempConnections.push_back(connection);
            i++;
        }

        for (auto con : tempConnections)
        {
            if (con[1] < 0) readElements[con[0]]->setRightConnection(nullptr);
            else readElements[con[0]]->setRightConnection(readElements[con[1]]);
            delete con;
        }


    } catch (...)
    {
        qDebug() << "LOAD ERROR";
        for (auto el : readElements)
        {
            delete el;
        }
        readElements.clear();
        return false;
    }

    return true;
}

void FileHandler::saveProject(std::string path, std::vector<SaprElement*> elements)
{
    std::ofstream fo;
    try
    {
        fo.open(path, std::ofstream::out | std::ofstream::trunc);
        for(int i = 0; i < elements.size(); i++)
        {
            fo << elements[i]->getLength() << " " << elements[i]->getSquare() << " " << elements[i]->getElasticModulus() << " ";
            fo << elements[i]->getPermissibleStress() << " " << elements[i]->getXLeftForce() << " " << elements[i]->getXRightForce() << " ";
            fo << elements[i]->getXQForce() << " " << elements[i]->hasLeftSupport() << " " << elements[i]->hasRightSupport() << " ";
            int rightBarId = -1;
            for (int j = 0; j < elements.size(); j++)
            {
                if (elements[j] == elements[i]->getRightConnectedElement()) rightBarId = j;
            }
            fo << rightBarId;
            fo << "\n";
        }
    } catch (...)
    {
    }

    if (fo.is_open()) fo.close();
}
