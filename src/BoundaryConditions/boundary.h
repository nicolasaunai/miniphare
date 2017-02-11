#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "vecfield/vecfield.h"
#include "grid/gridlayout.h"


class Boundary
{
public:
    virtual void applyMagneticBC(VecField& B, GridLayout const& layout) const = 0;
    virtual void applyElectricBC(VecField& E, GridLayout const& layout) const = 0;
    virtual void applyCurrentBC(VecField& J,  GridLayout const& layout) const = 0;
};

#endif // BOUNDARY_H
