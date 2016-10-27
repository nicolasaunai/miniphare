
#include <cmath>

#include "indexesandweightso4.h"


/**
 * @brief IndexesAndWeightsO4::computeWeights computes
 * the weights (or ponderations) associated to each point
 * of indexList_
 *
 * Inputs: indexList_, reducedCoord_
 * Output: weightList_
 *
 *
 */
void IndexesAndWeightsO4::computeWeights()
{
    weightList_[0] = (1./24.)*std::pow(5./2. + static_cast<double>(indexList_[0]) - reducedCoord_, 4.) ;

    weightList_[1] = (1./96.)*(55.
    - 20.         *(static_cast<double>(indexList_[1]) - reducedCoord_)
    - 120.*std::pow(static_cast<double>(indexList_[1]) - reducedCoord_, 2.)
    - 80. *std::pow(static_cast<double>(indexList_[1]) - reducedCoord_, 3.)
    - 16. *std::pow(static_cast<double>(indexList_[1]) - reducedCoord_, 4.) ) ;

    weightList_[2] = (1./192.)*(115.
    - 120.*std::pow(static_cast<double>(indexList_[2]) - reducedCoord_, 2.)
    + 48. *std::pow(static_cast<double>(indexList_[2]) - reducedCoord_, 4.) ) ;

    weightList_[3] = (1./96.)*(55.
    + 20.         *(static_cast<double>(indexList_[3]) - reducedCoord_)
    - 120.*std::pow(static_cast<double>(indexList_[3]) - reducedCoord_, 2.)
    + 80. *std::pow(static_cast<double>(indexList_[3]) - reducedCoord_, 3.)
    - 16. *std::pow(static_cast<double>(indexList_[3]) - reducedCoord_, 4.) ) ;

    weightList_[4] = (1./24.)*std::pow(5./2. - (static_cast<double>(indexList_[4]) - reducedCoord_), 4.) ;

}

