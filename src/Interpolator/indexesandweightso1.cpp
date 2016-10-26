
#include <cmath>

#include "indexesandweightso1.h"


/**
 * @brief IndexesAndWeightsO1::computeWeights computes
 * the weights (or ponderations) associated to each point
 * of indexList_
 *
 * Inputs: indexList_, primalMeshCoord_
 * Output: weightList_
 *
 *
 */
void IndexesAndWeightsO1::computeWeights()
{
    weightList_[1] = meshCoord_ - static_cast<double> (indexList_[0]) ;

    weightList_[0] = 1. - indexList_[1] ;

}

