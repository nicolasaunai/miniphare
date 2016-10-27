
#include <cmath>

#include "indexesandweightso3.h"


/**
 * @brief IndexesAndWeightsO3::computeWeights computes
 * the weights (or ponderations) associated to each point
 * of indexList_
 *
 * Inputs: indexList_, reducedCoord_
 * Output: weightList_
 *
 *
 */
void IndexesAndWeightsO3::computeWeights()
{
    weightList_[0] = (4./3.)*std::pow(1. + 0.5*(static_cast<double>(indexList_[0]) - reducedCoord_), 3.) ;

    weightList_[1] = 2./3. - std::pow(static_cast<double>(indexList_[1]) - reducedCoord_, 2.)
        - 0.5*std::pow(static_cast<double>(indexList_[1]) - reducedCoord_, 3.) ;

    weightList_[2] = 2./3. - std::pow(static_cast<double>(indexList_[2]) - reducedCoord_, 2.)
        + 0.5*std::pow(static_cast<double>(indexList_[2]) - reducedCoord_, 3.) ;

    weightList_[3] = (4./3.)*std::pow(1. - 0.5*(static_cast<double>(indexList_[3]) - reducedCoord_) , 3.) ;

}

