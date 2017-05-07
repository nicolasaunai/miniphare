
#include "Splitting/order3_rf2strategy.h"



const uint32 interpOrder  = 3;
const uint32 refineFactor = 2;


/**
 * @brief Order3_RF2Strategy::Order3_RF2Strategy is a
 * specific case of OrderN_RF2Strategy.
 * This splitting strategy is exact, it works for 3rd order and
 * a refinement factor of 2.
 *
 * See: miniphare/doc/html/md_markdown_exactSplitting_BSplines.html
 *
 * @param splitMethod
 */
Order3_RF2Strategy::Order3_RF2Strategy(const std::string& splitMethod)
    : SplittingStrategy(splitMethod,
                        (2 * refineFactor - 1) + (refineFactor - 1) * (interpOrder - 1))
{
    child_icellx_[0] = -2;
    child_icellx_[1] = -1;
    child_icellx_[2] = 0;
    child_icellx_[3] = 1;
    child_icellx_[4] = 2;

    child_deltax_[0] = 0.;
    child_deltax_[1] = 0.;
    child_deltax_[2] = 0.;
    child_deltax_[3] = 0.;
    child_deltax_[4] = 0.;

    child_weights_[0] = 1. / 8.;
    child_weights_[1] = 4. / 8.;
    child_weights_[2] = 6. / 8.;
    child_weights_[3] = 4. / 8.;
    child_weights_[4] = 1. / 8.;

    wtot_ = 2.;
}
