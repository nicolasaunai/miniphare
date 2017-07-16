
#include "domainboundarycondition.h"
#include "domainboundaryfactory.h"


/**
 * @brief DomainBoundaryCondition::DomainBoundaryCondition
 * we initialize isPatchBC = false
 *
 *
 * @param layout
 * @param boundaryInfos
 */
DomainBoundaryCondition::DomainBoundaryCondition(GridLayout layout,
                                                 std::vector<BoundaryInfo> boundaryInfos)
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
        bc->applyCurrentBC(J, layout_);
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



void DomainBoundaryCondition::applyOutgoingParticleBC(
    std::vector<Particle>& particleArray, LeavingParticles const& leavingParticles) const
{
    for (auto&& bc : boundaries_)
    {
        bc->applyOutgoingParticleBC(particleArray, leavingParticles);
    }
}


void DomainBoundaryCondition::applyIncomingParticleBC(Ions& ions, std::string const& pusher,
                                                      double const& dt) const
{
}



DomainBoundaryCondition::~DomainBoundaryCondition()
{
}
