
#include "hybridenums.h"

#include "ampereimpl1d.h"


/**
 * @brief AmpereImpl1D::AmpereImpl1D
 *
 *
 * @param dt
 * @param layout
 */
AmpereImpl1D::AmpereImpl1D(double dt, GridLayout const& layout)
    :AmpereImplInternals(dt, layout),
      dxBz_( layout.allocSizeDerived(HybridQuantity::Bz, Direction::X), HybridQuantity::Ey, "dxBz"),
      dxBy_( layout.allocSizeDerived(HybridQuantity::By, Direction::X), HybridQuantity::Ez, "dxBy")
{
}


void AmpereImpl1D::operator()(VecField const& B, VecField& Jnew)
{
    const Field& By  = B.component(VecField::VecY);
    const Field& Bz  = B.component(VecField::VecZ);

    Field& Jxnew    = Jnew.component(VecField::VecX);
    Field& Jynew    = Jnew.component(VecField::VecY);
    Field& Jznew    = Jnew.component(VecField::VecZ);

    layout_.deriv(Bz, Direction::X, dxBz_);
    layout_.deriv(By, Direction::X, dxBy_);

    uint32 iStart = layout_.physicalStartIndex(Jxnew, Direction::X);
    uint32 iEnd   = layout_.physicalEndIndex  (Jxnew, Direction::X);

    for (uint32 ix = iStart; ix <= iEnd; ++ix )
    {
        Jxnew(ix) = 0. ;
    }


    iStart = layout_.physicalStartIndex(Jynew, Direction::X);
    iEnd   = layout_.physicalEndIndex  (Jynew, Direction::X);

    for (uint32 ix = iStart; ix <= iEnd; ++ix )
    {
        Jynew(ix) = -dxBz_(ix) ;
    }


    iStart = layout_.physicalStartIndex(Jznew, Direction::X);
    iEnd   = layout_.physicalEndIndex  (Jznew, Direction::X);

    for (uint32 ix = iStart; ix <= iEnd; ++ix )
    {
        Jznew(ix) = dxBy_(ix) ;
    }

}

