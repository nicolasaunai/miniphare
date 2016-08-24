
#include <iostream>

#include "faraday1d.h"
#include "Vecfield/vecfield.h"




Faraday1D::Faraday1D(double dt, double dx)
    :Faraday{dt}, dx_{dx}, dtodx_(dx)
{
}




void Faraday1D::operator()(const VecField & E, const VecField & B, VecField& Bpred)
{
    std::cout << "got " << E.name() << " and "
              << B.name() << " and " << Bpred.name() << std::endl;


    const Field& Ey  = E.component(VecField::VecY);
    const Field& Ez  = E.component(VecField::VecZ);
    const Field& Bx  = B.component(VecField::VecX);
    const Field& By  = B.component(VecField::VecY);
    const Field& Bz  = B.component(VecField::VecZ);

    Field& BxPred    = Bpred.component(VecField::VecX);
    Field& ByPred    = Bpred.component(VecField::VecY);
    Field& BzPred    = Bpred.component(VecField::VecZ);


    std::vector<uint32> shape = B.shape();

    // does not account for ghost nodes etc. yet.
    // this should not really be dealt here anyway...
    for (uint32 ix = 0; ix < shape[0]; ix++ )
    {
        BxPred(ix) = Bx(ix);
        ByPred(ix) = By(ix) + dtodx_ * ( Ez(ix+1) - Ez(ix) );
        BzPred(ix) = Bz(ix) - dtodx_ * ( Ey(ix+1) - Ey(ix));
    }
}




