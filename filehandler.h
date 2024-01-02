#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "saprelement.h"

#include <iostream>
#include <vector>


class FileHandler
{
public:
    FileHandler();

    static void setFilePath(std::string path);
    static bool openProject(std::string path);
    static void createFile(std::string path);
    static void saveProject(std::vector<SaprElement*> elements);

    static std::vector<SaprElement*> readElements;

private:
    static std::string projectPath;
    static bool isPathSetted;
};

#endif // FILEHANDLER_H