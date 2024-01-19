#ifndef SAPRELEMENTDATA_H
#define SAPRELEMENTDATA_H

#include <vector>

struct SaprElementData
{
    static const int dataSize = 9;
    double length, square, elasticModulus, permissibleStress;
    double xRightForce, xLeftForce, xQForce;
    bool hasLeftSupport, hasRightSupport;

    bool dataMask[dataSize];

    void maskInit(std::vector<int> maskId)
    {
        for (int i = 0; i < dataSize; i++)
        {
            dataMask[i] = 0;
        }

        for (int id : maskId)
        {
            if (id > -1 && id < dataSize)
            {
                dataMask[id] = 1;
            }
        }
    }
};

#endif // SAPRELEMENTDATA_H
