

#include "approx_fujistrategy.h"



const uint32 nbpts = 2;

/**
 * @brief Approx_FujiStrategy::Approx_FujiStrategy only approximately preserve
 * the moments of the particle distribution on the coarse grid.
 *
 * In 1D an exact splitting algorithm should be prefered.
 *
 * https://hephaistos.lpp.polytechnique.fr/redmine/projects/hyb-par/wiki/ApproxSplit
 *
 *
 * @param splitMethod
 */
Approx_FujiStrategy::Approx_FujiStrategy(const std::string& splitMethod)
    : SplittingStrategy(splitMethod, nbpts)
    , jitterX_{0.}
{
    std::random_device randSeed;
    std::mt19937_64 generator(randSeed());

    // We limit jitter to 10% of the local cell size
    std::uniform_real_distribution<float> randPosX(0., 0.1f);

    // we prepare a random jitter for child particles
    jitterX_ = randPosX(generator);

    child_icellx_[0] = -1;
    child_icellx_[1] = 0;

    child_deltax_[0] = (1.0f - jitterX_);
    child_deltax_[1] = jitterX_;

    double weight     = 1. / nbpts;
    wtot_             = 1.;
    child_weights_[0] = weight;
    child_weights_[1] = weight;
}
