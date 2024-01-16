#ifndef SAPRELEMENTDATA_H
#define SAPRELEMENTDATA_H

struct SaprElementData
{
    double length, square, elasticModulus, permissibleStress;
    double xRightForce, xLeftForce, xQForce;
    bool hasLeftSupport, hasRightSupport;
};

#endif // SAPRELEMENTDATA_H
