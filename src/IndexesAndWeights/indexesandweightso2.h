#ifndef INDEXESANDWEIGHTSO2_H
#define INDEXESANDWEIGHTSO2_H



#include "indexesandweights.h"


class IndexesAndWeightsO2 : public IndexesAndWeights
{

public:
    ~IndexesAndWeightsO2() {}

    IndexesAndWeightsO2( uint32 order ): IndexesAndWeights(order) {}

    // the formulas ruling the weights are specific to a given order
    void computeWeights( double reducedCoord ) override ;
};


#endif // INDEXESANDWEIGHTSO2_H
