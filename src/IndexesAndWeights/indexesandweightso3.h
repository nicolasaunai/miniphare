#ifndef INDEXESANDWEIGHTSO3_H
#define INDEXESANDWEIGHTSO3_H



#include "indexesandweights.h"


class IndexesAndWeightsO3 : public IndexesAndWeights
{

public:
    ~IndexesAndWeightsO3() {}

    IndexesAndWeightsO3(uint32 order): IndexesAndWeights(order) {}

    // the formulas ruling the weights are specific to a given order
    /**
     * @brief IndexesAndWeightsO3::computeWeights computes
     * the weights (or ponderations) associated to each point
     * of indexList_
     *
     * Inputs: indexList_, reducedCoord_
     * Output: weightList_
     *
     * \f[
       \begin{equation}
       S_3(\xi) = \left\{ \begin{array}{ll}
       \frac{4}{3}\left( 1 + \frac{\xi}{2}\right)^3 & \textrm{ if } -2 \le \xi \le -1 \\
       -\frac{\xi^3}{2} -\xi^2 +2/3 & \textrm{ if }  -1 \le \xi \le 0 \\
        \frac{\xi^3}{2} -\xi^2 +2/3 & \textrm{ if }   0 \le \xi \le 1 \\
       \frac{4}{3}\left( 1 - \frac{\xi}{2}\right)^3 & \textrm{ if } 1 \le \xi \le 2 \\
           0 & \textrm{ elsewhere }
       \end{array} \right.
       \end{equation}
       \f]
     *
     *
     */
    virtual std::vector<double> const& computeWeights(double reducedCoord) final
    {
        double coef1, coef2, coef3, coef4;

        coef1 = 1. + 0.5*(static_cast<double>(indexList_[0]) - reducedCoord);
        coef2 = static_cast<double>(indexList_[1]) - reducedCoord;
        coef3 = static_cast<double>(indexList_[2]) - reducedCoord;
        coef4 = 1. - 0.5*(static_cast<double>(indexList_[3]) - reducedCoord);

        double coef2_sq  = coef2*coef2;
        double coef2_cub = coef2_sq*coef2;
        double coef3_sq  = coef3*coef3;
        double coef3_cub = coef3_sq*coef3;

        weightList_[0] = (4./3.)* coef1 * coef1 * coef1;
        weightList_[1] = 2./3. - coef2_sq  - 0.5*coef2_cub;
        weightList_[2] = 2./3. - coef3_sq + 0.5*coef3_cub;
        weightList_[3] = (4./3.)*coef4*coef4*coef4;

        return weightList_;
    }
};



#endif // INDEXESANDWEIGHTSO3_H
