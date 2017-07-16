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


void PatchBoundary::applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                            LeavingParticles const& leavingParticles) const
{
}


void PatchBoundary::applyIncomingParticleBC(BoundaryCondition const& temporaryBC, Pusher& pusher,
                                            GridLayout const& patchLayout,
                                            std::vector<Particle>& patchParticles, uint32 iesp)
{
}
