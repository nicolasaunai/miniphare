#ifndef DOMAINBOUNDARYCONDITION_H
#define DOMAINBOUNDARYCONDITION_H

#include <vector>

#include "BoundaryConditions/boundary_conditions.h"
#include "BoundaryConditions/boundary.h"
#include "vecfield/vecfield.h"
#include "grid/gridlayout.h"


class DomainBoundaryCondition : public BoundaryCondition
{
private:
    std::vector<std::unique_ptr<Boundary>> boundaries_;
    GridLayout layout_;

public:
    DomainBoundaryCondition(GridLayout layout) : layout_{std::move(layout)}{}

    virtual void applyMagneticBC(VecField& B) const override;
    virtual void applyElectricBC(VecField& E) const override;
    virtual void applyCurrentBC(VecField& J)  const override;
};


#endif // DOMAINBOUNDARYCONDITION_H
