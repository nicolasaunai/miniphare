#ifndef INDEXESANDWEIGHTSO1_H
#define INDEXESANDWEIGHTSO1_H



#include "indexesandweights.h"


class IndexesAndWeightsO1 : public IndexesAndWeights
{

public:
    ~IndexesAndWeightsO1() {}

    IndexesAndWeightsO1( uint32 order ): IndexesAndWeights(order) {}

    // the formulas ruling the weights are specific to a given order
    void computeWeights( double reducedCoord ) override ;
};


#endif // INDEXESANDWEIGHTSO1_H
