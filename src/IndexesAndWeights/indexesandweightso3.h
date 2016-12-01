#ifndef INDEXESANDWEIGHTSO3_H
#define INDEXESANDWEIGHTSO3_H



#include "indexesandweights.h"


class IndexesAndWeightsO3 : public IndexesAndWeights
{

public:
    ~IndexesAndWeightsO3() {}

    IndexesAndWeightsO3(uint32 order): IndexesAndWeights(order) {}

    // the formulas ruling the weights are specific to a given order
    void computeWeights( double reducedCoord ) override ;
};



#endif // INDEXESANDWEIGHTSO3_H
