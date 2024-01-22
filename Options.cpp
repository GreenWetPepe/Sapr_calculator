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

        static const int noConnection = 0;
        static const int leftConnection = 1;
        static const int rightConnection = 2;
    }

    namespace diagram
    {
        static double diagramSizeMultiply = 1.2;
        static double diagramIndentMultiply = 0.8;

        static double nXIndent = saprElement::paramIndentMultiply + diagramIndentMultiply + diagramSizeMultiply;
        static double uXIndent = saprElement::paramIndentMultiply + 2 * (diagramIndentMultiply + diagramSizeMultiply);
        static double sXIndent = saprElement::paramIndentMultiply + 3 * (diagramIndentMultiply + diagramSizeMultiply);

        static int pointsCount = 1500;
    }

    namespace previewProject
    {
        static const int width = 50;
        static const int height = 50;
    }

    namespace workSpace
    {
        static const int xElementConnectionSpread = 15;
        static const int yElementConnectionSpread = 100;
        static const double maxElementHeightRelationToWindow = 0.3;
        static const double maxElementWidthRelationToWindow = 0.3;
        static const double minElementHeightRelationToMaxHeight = 0.35;
        static const double minElementWidthRelationToMaxWidth = 0.35;

        static const int mouseMoveEventCallForStartMoveElement = 8;
    }
}
