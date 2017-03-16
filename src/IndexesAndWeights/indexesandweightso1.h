#ifndef INDEXESANDWEIGHTSO1_H
#define INDEXESANDWEIGHTSO1_H



#include "indexesandweights.h"


class IndexesAndWeightsO1 : public IndexesAndWeights
{

public:
    ~IndexesAndWeightsO1() {}

    IndexesAndWeightsO1( uint32 order ): IndexesAndWeights(order) {}

    /**
     * @brief IndexesAndWeightsO1::computeWeights computes
     * the weights (or ponderations) associated to each point
     * of indexList_
     *
     * Inputs: indexList_, reducedCoord_
     * Output: weightList_
     */
    inline virtual std::vector<double> const& computeWeights(double reducedCoord) final
    {
        weightList_[1] = reducedCoord - static_cast<double> (indexList_[0]) ;
        weightList_[0] = 1. - weightList_[1] ;
        return weightList_;
    }


};


#endif // INDEXESANDWEIGHTSO1_H
