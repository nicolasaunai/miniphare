#ifndef INDEXESANDWEIGHTSO4_H
#define INDEXESANDWEIGHTSO4_H



#include "indexesandweights.h"


class IndexesAndWeightsO4 : public IndexesAndWeights
{
public:
    ~IndexesAndWeightsO4() {}

    IndexesAndWeightsO4(uint32 order)
        : IndexesAndWeights(order)
    {
    }

    // the formulas ruling the weights are specific to a given order
    /**
     * @brief IndexesAndWeightsO4::computeWeights computes
     * the weights (or ponderations) associated to each point
     * of indexList_
     *
     * Inputs: indexList_, reducedCoord_
     * Output: weightList_
     *
     * \f[
       \begin{equation}
       S_4(\xi) = \left\{ \begin{array}{ll}
       \frac{1}{24}\left( \frac{5}{2} + \xi \right)^4 & \textrm{ if } -5/2 \le \xi \le -3/2 \\
       \frac{1}{96}\left( 55 - 20\xi - 120\xi^2 - 80 \xi^3 - 16 \xi^4 \right) & \textrm{ if } -3/2
     \le \xi \le -1/2 \\
       \frac{1}{192}\left( 115 - 120\xi^2 + 48\xi^4 \right) & \textrm{ if } -1/2 \le \xi \le 1/2 \\
       \frac{1}{96}\left( 55 + 20\xi - 120\xi^2 + 80 \xi^3 - 16 \xi^4 \right) & \textrm{ if } 1/2
     \le \xi \le 3/2 \\
       \frac{1}{24}\left( \frac{5}{2} - \xi \right)^4 & \textrm{ if } 3/2 \le \xi \le 5/2 \\
           0 & \textrm{ elsewhere }
       \end{array} \right.
       \end{equation}
     * \f]
     *
     */
    virtual void computeWeights(double reducedCoord, std::vector<uint32> const& indexList,
                                std::vector<double>& weightList) final
    {
        double w0_c = 5. / 2. + static_cast<double>(indexList[0]) - reducedCoord;

        double w1_c  = static_cast<double>(indexList[1]) - reducedCoord;
        double w1_c2 = w1_c * w1_c;
        double w1_c3 = w1_c2 * w1_c;
        double w1_c4 = w1_c3 * w1_c;

        double w2_c  = static_cast<double>(indexList[2]) - reducedCoord;
        double w2_c2 = w2_c * w2_c;
        double w2_c4 = w2_c2 * w2_c2;

        double w3_c  = static_cast<double>(indexList[3]) - reducedCoord;
        double w3_c2 = w3_c * w3_c;
        double w3_c3 = w3_c2 * w3_c;
        double w3_c4 = w3_c3 * w3_c;

        double w4_c  = 5. / 2. - (static_cast<double>(indexList[4]) - reducedCoord);
        double w4_c4 = w4_c * w4_c * w4_c * w4_c;

        weightList[0] = (1. / 24.) * w0_c * w0_c * w0_c * w0_c;
        weightList[1] = (1. / 96.) * (55. - 20 * w1_c - 120. * w1_c2 - 80. * w1_c3 - 16. * w1_c4);
        weightList[2] = (1. / 192.) * (115. - 120. * w2_c2 + 48. * w2_c4);
        weightList[3] = (1. / 96.) * (55. + 20 * w3_c - 120 * w3_c2 + 80 * w3_c3 - 16 * w3_c4);
        weightList[4] = (1. / 24.) * w4_c4;
    }
};


#endif // INDEXESANDWEIGHTSO4_H
