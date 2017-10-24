

#include "ohmimpl1d.h"
#include "utilities/types.h"



OhmImpl1D::OhmImpl1D(GridLayout const& layout, double eta, double nu)
    : OhmImpl{layout, eta, nu}
{
}


void OhmImpl1D::computeTerms(VecField const& B, Field const& Ne, VecField const& Ve,
                             Field const& Pe, VecField const& J)
{
    ideal_(Ve, B);
    resistive_(J);
    pressure_(Pe, Ne);
    // hyperResistivity(J);
}



OhmImpl1D::~OhmImpl1D()
{
}




/**
 * @brief calculate the -Ve x B term
 * @param Ve is the electron bulk velocity field
 * @param B is the magnetic field
 */
void OhmImpl1D::ideal_(VecField const& Ve, VecField const& B)
{
    // in the function we calculate the VexB term
    // since we don't know which layout we're in
    // we can't, here, put Ve on E, B on E and calculate VexB on E.

    // We we will therefore write the cross product in the most general way
    // where Ve and B are linear combinations of nodes
    // linear combinations being given by the GridLayout


    // this function is 1D therefore loop only in the X direction:

    Field const& Vex = Ve.component(VecField::VecX);
    Field const& Vey = Ve.component(VecField::VecY);
    Field const& Vez = Ve.component(VecField::VecZ);

    Field const& Bx = B.component(VecField::VecX);
    Field const& By = B.component(VecField::VecY);
    Field const& Bz = B.component(VecField::VecZ);

    Field& VexB_x = idealTerm_.component(VecField::VecX);
    Field& VexB_y = idealTerm_.component(VecField::VecY);
    Field& VexB_z = idealTerm_.component(VecField::VecZ);




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
    {
        uint32 const iStart = layout_.physicalStartIndex(VexB_x, Direction::X);
        uint32 const iEnd   = layout_.physicalEndIndex(VexB_x, Direction::X);

        for (uint32 ix = iStart; ix <= iEnd; ++ix)
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

            VexB_x(ix) = vzloc * byloc - vyloc * bzloc;
        }
    }



    // ------------------------------------------------------------------------
    //
    //                              Ve x B _ y
    //                           -(VzBx - VxBz)
    // ------------------------------------------------------------------------
    {
        uint32 const iStart = layout_.physicalStartIndex(VexB_y, Direction::X);
        uint32 const iEnd   = layout_.physicalEndIndex(VexB_y, Direction::X);

        for (uint32 ix = iStart; ix <= iEnd; ++ix)
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

            VexB_y(ix) = -vzloc * bxloc + vxloc * bzloc;
        }
    }



    // ------------------------------------------------------------------------
    //
    //                              Ve x B _ z
    //                           -(VxBy - VyBx)
    // ------------------------------------------------------------------------
    {
        uint32 const iStart = layout_.physicalStartIndex(VexB_z, Direction::X);
        uint32 const iEnd   = layout_.physicalEndIndex(VexB_z, Direction::X);

        for (uint32 ix = iStart; ix <= iEnd; ++ix)
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

            VexB_z(ix) = -vxloc * byloc + vyloc * bxloc;
        }
    }
}



/**
 * @brief Calculates the resistive term eta*J of the Ohm's law assuming constant eta
 * @param J is the total current density
 */
void OhmImpl1D::resistive_(VecField const& J)
{
    Field const& Jx = J.component(VecField::VecX);
    Field const& Jy = J.component(VecField::VecY);
    Field const& Jz = J.component(VecField::VecZ);

    Field& Rx = resistivityTerm_.component(VecField::VecX);
    Field& Ry = resistivityTerm_.component(VecField::VecY);
    Field& Rz = resistivityTerm_.component(VecField::VecZ);

    {
        uint32 const iStart = layout_.physicalStartIndex(Rx, Direction::X);
        uint32 const iEnd   = layout_.physicalEndIndex(Rx, Direction::X);


        for (uint32 ix = iStart; ix <= iEnd; ++ix)
        {
            Rx(ix) = Jx(ix) * eta_;
        }
    }

    {
        uint32 const iStart = layout_.physicalStartIndex(Ry, Direction::X);
        uint32 const iEnd   = layout_.physicalEndIndex(Ry, Direction::X);

        for (uint32 ix = iStart; ix <= iEnd; ++ix)
        {
            Ry(ix) = Jy(ix) * eta_;
        }
    }


    {
        uint32 const iStart = layout_.physicalStartIndex(Rz, Direction::X);
        uint32 const iEnd   = layout_.physicalEndIndex(Rz, Direction::X);

        for (uint32 ix = iStart; ix <= iEnd; ++ix)
        {
            Rz(ix) = Jz(ix) * eta_;
        }
    }
}




/**
 * @brief calculates the pressure gradient in 1D
 * @param Pe is the electron pressure field (scalar)
 */
void OhmImpl1D::pressure_(Field const& Pe, Field const& Ne)
{
    Field& gradPx = pressureTerm_.component(VecField::VecX);
    layout_.deriv(Pe, Direction::X, gradPx);

    // we need now to divide the gradPe by the electron density Ne
    // Ne is on primal^3 and gradPx is on the electric field
    // since we don't know here where the electric field is
    // we will express Ne in terms of a linear combination
    // of surrounding points the coefficients and nodes
    // indexes will be given by:
    LinearCombination const& avgPointsMomentsEx = layout_.momentsToEx();

    uint32 iStart = layout_.physicalStartIndex(gradPx, Direction::X);
    uint32 iEnd   = layout_.physicalEndIndex(gradPx, Direction::X);

    // we loop from iStart to iEnd and forget about ghost nodes
    // because they will be fixed by the boundary condition
    // on the electric field
    for (uint32 ix = iStart; ix <= iEnd; ++ix)
    {
        double ne_loc = 0;
        for (WeightPoint const& wp : avgPointsMomentsEx)
        {
            ne_loc += wp.coef * Ne(ix + wp.ix);
        }
        gradPx(ix) = -gradPx(ix) / ne_loc;
    }
}
