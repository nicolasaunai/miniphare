
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
    :FaradayImplInternals(dt,layout),
      dxEz_( layout.nx(), layout.ny(), layout.nz(), "_dxEz"),
      dxEy_( layout.nx(), layout.ny(), layout.nz(), "_dxEy")
{
}




void FaradayImpl1D::operator()(VecField const& E, VecField const& B, VecField& Bnew)
{
    std::cout << "got " << E.name() << " and "
              << B.name() << " and " << Bnew.name() << std::endl;

    double dtodx = dt_  / layout_.dx();

    const Field& Ey  = E.component(VecField::VecY);
    const Field& Ez  = E.component(VecField::VecZ);
    const Field& Bx  = B.component(VecField::VecX);
    const Field& By  = B.component(VecField::VecY);
    const Field& Bz  = B.component(VecField::VecZ);

    Field& Bxnew    = Bnew.component(VecField::VecX);
    Field& Bynew    = Bnew.component(VecField::VecY);
    Field& Bznew    = Bnew.component(VecField::VecZ);


    std::vector<uint32> shape = B.shape();

    // does not account for ghost nodes etc. yet.
    // this should not really be dealt here anyway...
    for (uint32 ix = 0; ix < shape[0]; ix++ )
    {
        Bxnew(ix) = Bx(ix);
        Bynew(ix) = By(ix) + dtodx * ( Ez(ix+1) - Ez(ix) );
        Bznew(ix) = Bz(ix) - dtodx * ( Ey(ix+1) - Ey(ix));
    }
}
