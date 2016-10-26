
#include <cmath>

#include "indexesandweightso4.h"


/**
 * @brief IndexesAndWeightsO4::computeWeights computes
 * the weights (or ponderations) associated to each point
 * of indexList_
 *
 * Inputs: indexList_, primalMeshCoord_
 * Output: weightList_
 *
 *
 */
void IndexesAndWeightsO4::computeWeights()
{
    weightList_[0] = (1./24.)*std::pow(5./2. + static_cast<double>(indexList_[0]) - meshCoord_, 4.) ;

    weightList_[1] = (1./96.)*(55.
    - 20.         *(static_cast<double>(indexList_[1]) - meshCoord_)
    - 120.*std::pow(static_cast<double>(indexList_[1]) - meshCoord_, 2.)
    - 80. *std::pow(static_cast<double>(indexList_[1]) - meshCoord_, 3.)
    - 16. *std::pow(static_cast<double>(indexList_[1]) - meshCoord_, 4.) ) ;

    weightList_[2] = (1./192.)*(115.
    - 120.*std::pow(static_cast<double>(indexList_[2]) - meshCoord_, 2.)
    + 48. *std::pow(static_cast<double>(indexList_[2]) - meshCoord_, 4.) ) ;

    weightList_[3] = (1./96.)*(55.
    + 20.         *(static_cast<double>(indexList_[3]) - meshCoord_)
    - 120.*std::pow(static_cast<double>(indexList_[3]) - meshCoord_, 2.)
    + 80. *std::pow(static_cast<double>(indexList_[3]) - meshCoord_, 3.)
    - 16. *std::pow(static_cast<double>(indexList_[3]) - meshCoord_, 4.) ) ;

    weightList_[4] = (1./24.)*std::pow(5./2. - (static_cast<double>(indexList_[4]) - meshCoord_), 4.) ;

}

