

#include "ohmimpl1d.h"
#include "types.h"



OhmImpl1D::OhmImpl1D(GridLayout const& layout)
    :OhmImpl{ layout }
{
}


void OhmImpl1D::operator()(VecField const& B, Field const& Ne,
                           VecField const& Ve, Field const& Pe,
                           VecField const&J, VecField& E)
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


    uint32 iStart    = layout_.physicalStartIndex(Ex, Direction::X);
    uint32 iEnd      = layout_.physicalEndIndex  (Ex, Direction::X);
    Field& ideal     = idealTerm_.component(VecField::VecX);
    Field& pressure  = pressureTerm_.component(VecField::VecX);

    for (uint32 ix=iStart; ix < iEnd; ++ix)
    {
     Ex(ix) = ideal(ix) + pressure(ix);// + resist_(ix) + hyperresist_(ix)
    }

    iStart    = layout_.physicalStartIndex(Ey, Direction::X);
    iEnd      = layout_.physicalEndIndex  (Ey, Direction::X);
    ideal     = idealTerm_.component(VecField::VecY);
    pressure  = pressureTerm_.component(VecField::VecY);

    for (uint32 ix=iStart; ix < iEnd; ++ix)
    {
        Ey(ix) = ideal(ix) + pressure(ix);// + resist_(ix) + hyperresist_(ix)
    }

    iStart    = layout_.physicalStartIndex(Ez, Direction::X);
    iEnd      = layout_.physicalEndIndex  (Ez, Direction::X);
    ideal     = idealTerm_.component(VecField::VecZ);
    pressure  = pressureTerm_.component(VecField::VecZ);


    for (uint32 ix=iStart; ix < iEnd; ++ix)
    {
        Ez(ix) = ideal(ix) + pressure(ix);// + resistivityTerm_(ix) + hyperresist_(ix)
    }

}



OhmImpl1D::~OhmImpl1D(){}





void OhmImpl1D::ideal_(VecField const& Ve, VecField const& B)
{
    // in the function we calculate the VexB term
    // since we don't know which layout we're in
    // we can't, here, put Ve on E, B on E and calculate VexB on E.

    // We we will therefore write the cross product in the most general way
    // where Ve and B are linear combinations of nodes
    // linear combinations being given by the GridLayout


    // this function is 1D therefore loop only in the X direction:

    Field const& Vex = Ve.component(0);
    Field const& Vey = Ve.component(1);
    Field const& Vez = Ve.component(2);

    Field const& Bx  = B.component(0);
    Field const& By  = B.component(1);
    Field const& Bz  = B.component(2);

    Field& VexB_x = idealTerm_.component(0);
    Field& VexB_y = idealTerm_.component(1);
    Field& VexB_z = idealTerm_.component(2);




    // here we get the relative indices and weights to average the moments
    // and the magnetic field components onto the electric field
    LinearCombination const& avgPointsMomentsEx = layout_.momentsToEx();
    LinearCombination const& avgPointsBzEx      = layout_.BzToEx();
    LinearCombination const& avgPointsByEx      = layout_.ByToEx();

    LinearCombination const& avgPointsMomentsEy = layout_.momentsToEy();
    LinearCombination const& avgPointsBxEy      = layout_.BxToEy();
    LinearCombination const& avgPointsBzEy      = layout_.BzToEy();

    LinearCombination const& avgPointsMomentsEz = layout_.momentsToEz();
    LinearCombination const& avgPointsBxEz      = layout_.BxToEz();
    LinearCombination const& avgPointsByEz      = layout_.ByToEz();


    // ------------------------------------------------------------------------
    //
    //                              Ve x B _ x
    //                           -(VyBz - VzBy)
    // ------------------------------------------------------------------------
    uint32 const iStartx = layout_.physicalStartIndex(VexB_x, Direction::X);
    uint32 const iEndx   = layout_.physicalEndIndex(VexB_x,  Direction::X);

    for (uint32 ix=iStartx; ix <= iEndx; ++ix)
    {

        // get Vz at 'ix'
        // and Vy at 'ix'
        double vyloc = 0;
        double vzloc = 0;
        for (WeightPoint const& wp : avgPointsMomentsEx)
        {
            vyloc += wp.coef * Vey(ix + wp.ix);
            vzloc += wp.coef * Vez(ix + wp.ix);
        }

        // get Bz at 'ix'
        double bzloc = 0;
        for (WeightPoint const& wp : avgPointsBzEx)
        {
            bzloc += wp.coef * Bz(ix + wp.ix);
        }

        // get By at 'ix'
        double byloc = 0;
        for (WeightPoint const& wp : avgPointsByEx)
        {
            byloc += wp.coef * By(ix + wp.ix);
        }

        VexB_x(ix) = vzloc * byloc  -  vyloc * bzloc;
    }



    // ------------------------------------------------------------------------
    //
    //                              Ve x B _ y
    //                           -(VzBx - VxBz)
    // ------------------------------------------------------------------------
    uint32 const iStarty = layout_.physicalStartIndex(VexB_y, Direction::X);
    uint32 const iEndy   = layout_.physicalEndIndex(VexB_y,  Direction::X);

    for (uint32 ix=iStarty; ix <= iEndy; ++ix)
    {
        // get Vz at 'ix'
        double vzloc = 0;
        for (WeightPoint const& wp : avgPointsMomentsEy)
        {
            vzloc += wp.coef * Vez(ix + wp.ix);
        }

        // get Bx at 'ix'
        double bxloc = 0;
        for (WeightPoint const& wp : avgPointsBxEy)
        {
            bxloc += wp.coef * Bx(ix + wp.ix);
        }

        // get Vx at 'ix'
        double vxloc = 0;
        for (WeightPoint const& wp : avgPointsMomentsEy)
        {
            vxloc += wp.coef * Vex(ix + wp.ix);
        }

        // get Bz at 'ix'
        double bzloc = 0;
        for (WeightPoint const& wp : avgPointsBzEy)
        {
            bzloc += wp.coef * Bz(ix + wp.ix);
        }

        VexB_y(ix) = - vzloc * bxloc  +  vxloc * bzloc;
    }



    // ------------------------------------------------------------------------
    //
    //                              Ve x B _ z
    //                           -(VxBy - VyBx)
    // ------------------------------------------------------------------------
    uint32 const iStartz = layout_.physicalStartIndex(VexB_z, Direction::X);
    uint32 const iEndz   = layout_.physicalEndIndex(VexB_z,  Direction::X);

    for (uint32 ix=iStartz; ix <= iEndz; ++ix)
    {
        // get Vx at 'ix'
        double vxloc = 0;
        for (WeightPoint const& wp : avgPointsMomentsEz)
        {
            vxloc += wp.coef * Vex(ix + wp.ix);
        }

        // get By at 'ix'
        double byloc = 0;
        for (WeightPoint const& wp : avgPointsByEz)
        {
            byloc += wp.coef * By(ix + wp.ix);
        }

        // get Vy at 'ix'
        double vyloc = 0;
        for (WeightPoint const& wp : avgPointsMomentsEz)
        {
            vyloc += wp.coef * Vey(ix + wp.ix);
        }

        // get Bx at 'ix'
        double bxloc = 0;
        for (WeightPoint const& wp : avgPointsBxEz)
        {
            bxloc += wp.coef * Bx(ix + wp.ix);
        }

        VexB_z(ix) = vxloc * byloc  -  vyloc * bxloc;
    }

}



























