#include "BoundaryConditions/patchboundary.h"



void PatchBoundary::initPRAParticles()
{
    // we trigger actual loading of particles
    ions_.loadParticles();
}


void PatchBoundary::applyElectricBC(VecField& E, GridLayout const& layout) const
{
}


void PatchBoundary::applyMagneticBC(VecField& B, GridLayout const& layout) const
{
}


void PatchBoundary::applyCurrentBC(VecField& J, GridLayout const& layout) const
{
}


void PatchBoundary::applyDensityBC(Field& J, GridLayout const& layout) const
{
}


void PatchBoundary::applyBulkBC(VecField& Vi, GridLayout const& layout) const
{
}


void PatchBoundary::applyParticleBC(std::vector<Particle>& particleArray,
                                    LeavingParticles const& leavingParticles) const
{
}
