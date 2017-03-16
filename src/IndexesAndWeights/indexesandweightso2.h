#ifndef INDEXESANDWEIGHTSO2_H
#define INDEXESANDWEIGHTSO2_H



#include "indexesandweights.h"


class IndexesAndWeightsO2 : public IndexesAndWeights
{

public:
    ~IndexesAndWeightsO2() {}

    IndexesAndWeightsO2( uint32 order ): IndexesAndWeights(order) {}

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
    virtual std::vector<double> const& computeWeights( double reducedCoord ) final
    {
        double coef1, coef2, coef3;

        coef1 = 0.5 + (static_cast<double>(indexList_[1]) - reducedCoord);
        coef2 = static_cast<double>(indexList_[1]) - reducedCoord;
        coef3 = 0.5 - (static_cast<double>(indexList_[1]) - reducedCoord);

        weightList_[0] = 0.5 * coef1 * coef1;
        weightList_[1] = 0.75 -coef2 * coef2;
        weightList_[2] = 0.5 * coef3 * coef3;

        return weightList_;
    }
};


#endif // INDEXESANDWEIGHTSO2_H
