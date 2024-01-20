#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "saprelement.h"

#include <iostream>
#include <vector>


class FileHandler
{
public:
    FileHandler();

    static bool openProject(std::string path);
    static void createFile(std::string path);
    static void saveProject(std::string path, std::vector<SaprElement*> elements);

    static std::vector<SaprElement*> readElements;

private:
};

#endif // FILEHANDLER_H
