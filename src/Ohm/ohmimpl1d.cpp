

#include "ohmimpl1d.h"
#include "types.h"



OhmImpl1D::OhmImpl1D(GridLayout const& layout)
    :OhmImplInternals{ layout }
{
}


void OhmImpl1D::operator()(VecField const& B, Field const& Ne,
                           VecField const& Ve, Field const& Pe, VecField& E)
{

    const Field& Bx  = B.component(VecField::VecX);
    const Field& By  = B.component(VecField::VecY);
    const Field& Bz  = B.component(VecField::VecZ);

    Field& Ex    = E.component(VecField::VecX);
    Field& Ey    = E.component(VecField::VecY);
    Field& Ez    = E.component(VecField::VecZ);


    // R<Rscalar> myRes1;
    // R<Rfield>  myRes2;

    // calculate ideal
    // " "   pressure
    // "" resist.


    uint32 iStart    = layout_.physicalStartIndex(Ex, Direction::directionX);
    uint32 iEnd      = layout_.physicalEndIndex  (Ex, Direction::directionX);
    Field& ideal     = idealTerm_.component(VecField::VecX);
    Field& pressure  = pressureTerm_.component(VecField::VecX);

    for (uint32 ix=iStart; ix < iEnd; ++ix)
    {
     Ex(ix) = ideal(ix) + pressure(ix);// + resist_(ix) + hyperresist_(ix)
    }

    iStart    = layout_.physicalStartIndex(Ey, Direction::directionX);
    iEnd      = layout_.physicalEndIndex  (Ey, Direction::directionX);
    ideal     = idealTerm_.component(VecField::VecY);
    pressure  = pressureTerm_.component(VecField::VecY);

    for (uint32 ix=iStart; ix < iEnd; ++ix)
    {
        Ey(ix) = ideal(ix) + pressure(ix);// + resist_(ix) + hyperresist_(ix)
    }

    iStart    = layout_.physicalStartIndex(Ez, Direction::directionX);
    iEnd      = layout_.physicalEndIndex  (Ez, Direction::directionX);
    ideal     = idealTerm_.component(VecField::VecZ);
    pressure  = pressureTerm_.component(VecField::VecZ);


    for (uint32 ix=iStart; ix < iEnd; ++ix)
    {
        Ez(ix) = ideal(ix) + pressure(ix);// + resistivityTerm_(ix) + hyperresist_(ix)
    }

}



  OhmImpl1D::~OhmImpl1D(){}
