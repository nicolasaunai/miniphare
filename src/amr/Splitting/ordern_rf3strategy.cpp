
#include "ordern_rf3strategy.h"

#include "multinomial.h"



const uint32 refineFactor = 3;

/**
 * @brief OrderN_RF3Strategy::OrderN_RF3Strategy
 * This splitting strategy is exact, it works for arbitrary order but
 * only when the refinement factor equals 3.
 * One mother Particle is split into:
 * 5, 7, 9, 11 children particles depending on order 1, 2, 3, 4
 *
 * See: miniphare/doc/html/md_markdown_exactSplitting_BSplines.html
 * for algebraic details.
 * The polynomial decomposition requires to compute trinomial coefficients
 * see https://en.wikipedia.org/wiki/Trinomial_triangle
 *
 * @param splitMethod
 * @param refineFactor
 * @param interpOrder
 */
OrderN_RF3Strategy::OrderN_RF3Strategy(const std::string& splitMethod, uint32 interpOrder)
    : SplittingStrategy(splitMethod,
                        (2 * refineFactor - 1) + (refineFactor - 1) * (interpOrder - 1))
{
    float nbrCello2 = static_cast<float>((nbpts_ - 1) / 2.);

    float delta = nbrCello2 - std::floor(nbrCello2);

    int32 icell = static_cast<int32>(std::floor(-nbrCello2));
    for (uint32 ik = 0; ik < nbpts_; ++ik)
    {
        child_icellx_[ik] = icell;
        ++icell;

        child_deltax_[ik] = delta;
    }

    uint32 itab = 0;
    for (int ik = -(static_cast<int>(interpOrder) + 1); ik < static_cast<int>(interpOrder) + 2;
         ik++)
    {
        child_weights_[itab] = static_cast<double>(trinomial(interpOrder + 1, ik))
                               / static_cast<double>(ipow(refineFactor, interpOrder));

        wtot_ += child_weights_[itab];

        itab++;
    }
}
