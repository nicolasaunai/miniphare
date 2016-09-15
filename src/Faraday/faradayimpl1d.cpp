
#include "hybridenums.h"

#include "faradayimpl1d.h"




FaradayImpl1D::FaradayImpl1D(double dt, GridLayout const& layout)
    :FaradayImplInternals(dt, layout),
      dxEz_( layout.allocSizeDerived(HybridQuantity::Ez, Direction::X).nx_ ,
             layout.allocSizeDerived(HybridQuantity::Ez, Direction::X).ny_ ,
             layout.allocSizeDerived(HybridQuantity::Ez, Direction::X).nz_ ,
             HybridQuantity::temporary, "_dxEz"),
      dxEy_( layout.allocSizeDerived(HybridQuantity::Ey, Direction::X).nx_ ,
             layout.allocSizeDerived(HybridQuantity::Ey, Direction::X).ny_ ,
             layout.allocSizeDerived(HybridQuantity::Ey, Direction::X).nz_ ,
             HybridQuantity::temporary, "_dxEy")
{
}


void FaradayImpl1D::operator()(VecField const& E, VecField const& B, VecField& Bnew)
{


    const Field& Ey  = E.component(VecField::VecY);
    const Field& Ez  = E.component(VecField::VecZ);
    const Field& Bx  = B.component(VecField::VecX);
    const Field& By  = B.component(VecField::VecY);
    const Field& Bz  = B.component(VecField::VecZ);

    Field& Bxnew    = Bnew.component(VecField::VecX);
    Field& Bynew    = Bnew.component(VecField::VecY);
    Field& Bznew    = Bnew.component(VecField::VecZ);


    // warning: we add a loop over the whole domain.
    // should be better than having one loop with polymorphic
    // derivative. (should be tested).
    layout_.deriv(Ez, Direction::X, dxEz_);
    layout_.deriv(Ey, Direction::X, dxEy_);



    uint32 iStart = layout_.physicalStartIndex(Bx, Direction::X);
    uint32 iEnd   = layout_.physicalEndIndex  (Bx, Direction::X);

    for (uint32 ix = iStart; ix < iEnd; ++ix )
    {
        Bxnew(ix) = Bx(ix);
    }


    iStart = layout_.physicalStartIndex(By, Direction::X);
    iEnd   = layout_.physicalEndIndex  (By, Direction::X);

    for (uint32 ix = iStart; ix < iEnd; ++ix )
    {
        Bynew(ix) +=   dt_ * dxEz_(ix);
    }


    iStart = layout_.physicalStartIndex(Bz, Direction::X);
    iEnd   = layout_.physicalEndIndex  (Bz, Direction::X);

    for (uint32 ix = iStart; ix < iEnd; ++ix )
    {
        Bznew(ix) += - dt_ * dxEy_(ix);
    }



}

