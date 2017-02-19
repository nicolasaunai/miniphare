#ifndef FIELDBC3DCOLLECTION_H
#define FIELDBC3DCOLLECTION_H

#include <vector>
#include <memory>

#include "vecfield/vecfield.h"
#include "Field/field.h"

enum class BoundaryType{ Periodic };


class BoundaryCondition
{
public:
    virtual void applyMagneticBC(VecField& B) const = 0;
    virtual void applyElectricBC(VecField& E) const = 0;
    virtual void applyCurrentBC(VecField& J)  const = 0;
    virtual void applyDensityBC(Field& N)     const = 0;
    virtual void applyBulkBC(VecField& Vi)    const = 0;

    virtual ~BoundaryCondition() = 0;
};



#endif // FIELDBC3DCOLLECTION_H
