
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
    weightList_[0] = 0.5*std::pow( 0.5 + (static_cast<double>(indexList_[1]) - reducedCoord), 2.) ;

    weightList_[1] = 0.75 - std::pow( static_cast<double>(indexList_[1]) - reducedCoord, 2.) ;

    weightList_[2] = 0.5*std::pow( 0.5 - (static_cast<double>(indexList_[1]) - reducedCoord), 2.) ;

}

