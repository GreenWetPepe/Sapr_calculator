#include <iostream>

namespace options
{
    namespace saprElement
    {
        static double sizeMultiply = 1;
        static double paramIndentMultiply = 0.7;

        static const int qForceArrowIndent = 10;
        static const int xQForceArrowIndent = 7, yQForceArrowIndent = 5;
        constexpr static double forceArrowIndentCoeff = 0.4;

    }

    namespace diagram
    {
        static double diagramSizeMultiply = 1.2;
        static double diagramIndentMultiply = 0.6;

        static double nXIndent = saprElement::paramIndentMultiply + diagramIndentMultiply + diagramSizeMultiply;
        static double uXIndent = saprElement::paramIndentMultiply + 2 * (diagramIndentMultiply + diagramSizeMultiply);
        static double sXIndent = saprElement::paramIndentMultiply + 3 * (diagramIndentMultiply + diagramSizeMultiply);

        static int pointsCount = 1500;
    }

}
