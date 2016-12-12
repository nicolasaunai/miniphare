#ifndef INDEXESANDWEIGHTSO4_H
#define INDEXESANDWEIGHTSO4_H



#include "indexesandweights.h"


class IndexesAndWeightsO4 : public IndexesAndWeights
{

public:
    ~IndexesAndWeightsO4() {}

    IndexesAndWeightsO4( uint32 order ): IndexesAndWeights(order) {}

    // the formulas ruling the weights are specific to a given order
    void computeWeights( double reducedCoord ) override ;
};


#endif // INDEXESANDWEIGHTSO4_H
