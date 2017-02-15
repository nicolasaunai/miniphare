#ifndef FIELDBC3DCOLLECTION_H
#define FIELDBC3DCOLLECTION_H

#include <vector>
#include <memory>

#include "vecfield/vecfield.h"


class BoundaryCondition
{
public:
    virtual void applyMagneticBC(VecField& B) const = 0;
    virtual void applyElectricBC(VecField& E) const = 0;
    virtual void applyCurrentBC(VecField& J)  const = 0;

    virtual ~BoundaryCondition() = 0;
};



#endif // FIELDBC3DCOLLECTION_H
