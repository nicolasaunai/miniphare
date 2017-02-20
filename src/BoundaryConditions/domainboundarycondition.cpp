
#include "domainboundarycondition.h"
#include "domainboundaryfactory.h"


DomainBoundaryCondition::DomainBoundaryCondition(GridLayout layout, std::vector<BoundaryInfo> boundaryInfos)
: layout_{std::move(layout)}
{
    for (BoundaryInfo boundary : boundaryInfos)
    {
        boundaries_.push_back(DomainBoundaryFactory::makeBoundary(boundary));
    }
}



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



void DomainBoundaryCondition::applyDensityBC(Field& Ni) const
{
    for (auto&& bc : boundaries_)
    {
        bc->applyDensityBC(Ni, layout_);
    }
}




void DomainBoundaryCondition::applyBulkBC(VecField& Vi) const
{
    for (auto&& bc : boundaries_)
    {
        bc->applyBulkBC(Vi, layout_);
    }
}



void DomainBoundaryCondition::applyParticleBC(std::vector<Particle>& particleArray) const
{

}


