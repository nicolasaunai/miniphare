
#include "faraday.h"
#include "faradayfactory.h"
#include "grid/gridlayout.h"


Faraday::Faraday(double dt, GridLayout const& layout)
    : implPtr_{FaradayImplFactory::createFaradayImpl(dt, layout)}
{
}




void Faraday::operator()(VecField const& E, VecField const& B, VecField& Bnew)
{
    return (*implPtr_)(E, B, Bnew);
}





FaradayImpl1D::FaradayImpl1D(double dt, GridLayout const& layout)
    :FaradayImplInternals(dt, layout),
      dxEz_( layout.nx(), layout.ny(), layout.nz(), "_dxEz"),
      dxEy_( layout.nx(), layout.ny(), layout.nz(), "_dxEy")
{
}



FaradayImpl::~FaradayImpl(){} // why here



void FaradayImpl1D::operator()(VecField const& E, VecField const& B, VecField& Bnew)
{
    std::cout << "got " << E.name() << " and "
              << B.name() << " and " << Bnew.name() << std::endl;

    //double dtodx = dt_  / layout_.dx();

    const Field& Ey  = E.component(VecField::VecY);
    const Field& Ez  = E.component(VecField::VecZ);
    const Field& Bx  = B.component(VecField::VecX);
    const Field& By  = B.component(VecField::VecY);
    const Field& Bz  = B.component(VecField::VecZ);

    Field& Bxnew    = Bnew.component(VecField::VecX);
    Field& Bynew    = Bnew.component(VecField::VecY);
    Field& Bznew    = Bnew.component(VecField::VecZ);


    //std::vector<uint32> shape = B.shape();


    layout_.deriv(Ez, GridLayout::directionX, dxEz_);
    layout_.deriv(Ey, GridLayout::directionX, dxEy_);



    uint32 iStart = layout_.physicalStartIndex(Bx, GridLayout::directionX);
    uint32 iEnd   = layout_.physicalEndIndex  (Bx, GridLayout::directionX);

    for (uint32 ix = iStart; ix < iEnd; ++ix )
    {
        Bxnew(ix) = Bx(ix);
    }


    iStart = layout_.physicalStartIndex(By, GridLayout::directionX);
    iEnd   = layout_.physicalEndIndex  (By, GridLayout::directionX);

    for (uint32 ix = iStart; ix < iEnd; ++ix )
    {
        Bynew(ix) +=   dt_ * dxEz_(ix);
    }


    iStart = layout_.physicalStartIndex(Bz, GridLayout::directionX);
    iEnd   = layout_.physicalEndIndex  (Bz, GridLayout::directionX);

    for (uint32 ix = iStart; ix < iEnd; ++ix )
    {
        Bznew(ix) += - dt_ * dxEy_(ix);
    }



}













