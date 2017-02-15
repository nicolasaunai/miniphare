#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "vecfield/vecfield.h"
#include "grid/gridlayout.h"

/**
 * @brief The Boundary class is an interface for applying boundary conditions in solveStep
 */
class Boundary
{
public:
    virtual void applyMagneticBC(VecField& B, GridLayout const& layout) const = 0;
    virtual void applyElectricBC(VecField& E, GridLayout const& layout) const = 0;
    virtual void applyCurrentBC(VecField& J,  GridLayout const& layout) const = 0;
    virtual void applyDensityBC(Field& J,     GridLayout const& layout) const = 0;
    virtual void applyBulkBC(VecField& Vi,    GridLayout const& layout) const = 0;
};

#endif // BOUNDARY_H
