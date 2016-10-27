#ifndef INDEXESANDWEIGHTSO1_H
#define INDEXESANDWEIGHTSO1_H



#include "indexesandweights.h"


class IndexesAndWeightsO1 : public IndexesAndWeights
{

public:
    ~IndexesAndWeightsO1() {}

    IndexesAndWeightsO1( uint32 order, double ods, double sminGlobal )
        : IndexesAndWeights(order, ods, sminGlobal) {}

    // the formulas ruling the weights are specific to a given order
    void computeWeights() override ;
};


#endif // INDEXESANDWEIGHTSO1_H
