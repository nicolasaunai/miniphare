#include "domainboundarycondition.h"


void DomainBoundaryCondition::applyMagneticBC(VecField& B) const
{
    for (auto&& bc : boundaries_)
    {
        bc->applyMagneticBC(B, layout_);
    }
}


void DomainBoundaryCondition::applyElectricBC(VecField& E) const
{
    for (auto&& bc : boundaries_)
    {
        bc->applyElectricBC(E, layout_);
    }
}


void DomainBoundaryCondition::applyCurrentBC(VecField& J) const
{
    for (auto&& bc : boundaries_)
    {
        bc->applyMagneticBC(J, layout_);
    }
}


