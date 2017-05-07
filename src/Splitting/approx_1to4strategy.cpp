

#include "Splitting/approx_1to4strategy.h"




const uint32 nbpts = 4;

/**
 * @brief Approx_1to4Strategy::Approx_1to4Strategy only approximately preserve
 * the moments of the particle distribution on the coarse grid.
 *
 * In 1D an exact splitting algorithm should be prefered.
 *
 * https://hephaistos.lpp.polytechnique.fr/redmine/projects/hyb-par/wiki/ApproxSplit
 *
 * @param splitMethod
 */
Approx_1to4Strategy::Approx_1to4Strategy(const std::string& splitMethod)
    : SplittingStrategy(splitMethod, nbpts)
    , shiftX_{0.}
{
    std::random_device randSeed;
    std::mt19937_64 generator(randSeed());

    // We limit jitter to 10% of the local cell size
    std::uniform_real_distribution<float> randPosX(0., 0.1f);

    // we prepare a random shift for child particles
    shiftX_ = randPosX(generator);

    child_icellx_[0] = -1;
    child_icellx_[1] = -1;
    child_icellx_[2] = 0;
    child_icellx_[3] = 0;

    child_deltax_[0] = (1.0f - 1.5f * shiftX_);
    child_deltax_[1] = (1.0f - 0.5f * shiftX_);
    child_deltax_[2] = 0.5f * shiftX_;
    child_deltax_[3] = 1.5f * shiftX_;

    double weight = 1. / nbpts;
    wtot_         = 1.;
    for (uint32 ipart         = 0; ipart < nbpts; ++ipart)
        child_weights_[ipart] = weight;
}
