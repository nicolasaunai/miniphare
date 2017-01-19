
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
void IndexesAndWeightsO4::computeWeights( double reducedCoord )
{
    double w0_c = 5./2. + static_cast<double>(indexList_[0]) - reducedCoord;

    double w1_c = static_cast<double>(indexList_[1]) - reducedCoord;
    double w1_c2 = w1_c*w1_c;
    double w1_c3 = w1_c2*w1_c;
    double w1_c4 = w1_c3*w1_c;

    double w2_c = static_cast<double>(indexList_[2]) - reducedCoord;
    double w2_c2 = w2_c * w2_c;
    double w2_c4 = w2_c2 * w2_c2;

    double w3_c = static_cast<double>(indexList_[3]) - reducedCoord;
    double w3_c2 = w3_c * w3_c;
    double w3_c3 = w3_c2 * w3_c;
    double w3_c4 = w3_c3 * w3_c;

    double w4_c = 5./2. - (static_cast<double>(indexList_[4]) - reducedCoord);
    double w4_c4 = w4_c * w4_c * w4_c * w4_c;


    weightList_[0] = (1./24.)* w0_c*w0_c*w0_c*w0_c;
    weightList_[1] = (1./96.)*(55.- 20 *w1_c - 120.* w1_c2 - 80. * w1_c3 - 16. * w1_c4);
    weightList_[2] = (1./192.)*(115.- 120.*w2_c2 + 48. * w2_c4);
    weightList_[3] = (1./96.)*(55. + 20*w3_c -  120*w3_c2 + 80*w3_c3 - 16*w3_c4);
    weightList_[4] = (1./24.) * w4_c4;

}

