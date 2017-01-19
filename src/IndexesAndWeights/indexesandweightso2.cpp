
#include <cmath>

#include "indexesandweightso2.h"


/**
 * @brief IndexesAndWeightsO2::computeWeights computes
 * the weights (or ponderations) associated to each point
 * of indexList_
 *
 * Inputs: indexList_, reducedCoord_
 * Output: weightList_
 *
 *
 */
void IndexesAndWeightsO2::computeWeights( double reducedCoord )
{
    double coef1, coef2, coef3;

    coef1 = 0.5 + (static_cast<double>(indexList_[1]) - reducedCoord);
    coef2 = static_cast<double>(indexList_[1]) - reducedCoord;
    coef3 = 0.5 - (static_cast<double>(indexList_[1]) - reducedCoord);

    weightList_[0] = 0.5 * coef1 * coef1;

    weightList_[1] = 0.75 -coef2 * coef2;

    weightList_[2] = 0.5 * coef3 * coef3;

}

