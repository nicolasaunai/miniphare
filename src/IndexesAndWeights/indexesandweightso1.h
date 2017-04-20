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
     *
     * \f[
       \begin{equation}
       S_1(\xi) = \left\{ \begin{array}{ll}
       \xi+1 & \textrm{ if } -1 \le \xi \le 0 \\
       1-\xi & \textrm{ if }  0 \le \xi \le 1 \\
           0 & \textrm{ elsewhere }
       \end{array} \right.
       \end{equation}
       \f]
     *
     * Some tricks are used to optimize computation
     *
     */
    inline virtual std::vector<double> const& computeWeights(double reducedCoord) final
    {
        weightList_[1] = reducedCoord - static_cast<double> (indexList_[0]) ;
        weightList_[0] = 1. - weightList_[1] ;
        return weightList_;
    }


};


#endif // INDEXESANDWEIGHTSO1_H
