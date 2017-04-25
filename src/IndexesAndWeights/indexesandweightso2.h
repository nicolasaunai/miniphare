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
     * \f[
       \begin{equation}
       S_2(\xi) = \left\{ \begin{array}{ll}
       \frac{1}{2}\left( \frac{3}{2} + \xi\right)^2 & \textrm{ if } -3/2 \le \xi \le -1/2 \\
       \frac{3}{4} -\xi^2 & \textrm{ if }  -1/2 \le \xi \le 1/2 \\
       \frac{1}{2}\left( \frac{3}{2} - \xi\right)^2 & \textrm{ if } 1/2 \le \xi \le 3/2 \\
           0 & \textrm{ elsewhere }
       \end{array} \right.
       \end{equation}
       \f]
     *
     *
     */
    virtual void computeWeights( double reducedCoord, std::vector<uint32> const&indexList,
                                    std::vector<double> & weightList) final
    {
        double coef1, coef2, coef3;

        coef1 = 0.5 + (static_cast<double>(indexList[1]) - reducedCoord);
        coef2 = static_cast<double>(indexList[1]) - reducedCoord;
        coef3 = 0.5 - (static_cast<double>(indexList[1]) - reducedCoord);

        weightList[0] = 0.5 * coef1 * coef1;
        weightList[1] = 0.75 -coef2 * coef2;
        weightList[2] = 0.5 * coef3 * coef3;

    }
};


#endif // INDEXESANDWEIGHTSO2_H
