#include "frozenboundary.h"


void FrozenBoundary::applyElectricBC(VecField& E, GridLayout const& layout) const
{
    (void)E;
    (void)layout;
    // no implementation
}



void FrozenBoundary::applyMagneticBC(VecField& B, GridLayout const& layout) const
{
    (void)B;
    (void)layout;
    // no implementation
}


void FrozenBoundary::applyCurrentBC(VecField& J, GridLayout const& layout) const
{
    (void)J;
    (void)layout;
    // no implementation
}


void FrozenBoundary::applyDensityBC(Field& J, GridLayout const& layout) const
{
    (void)J;
    (void)layout;
    // no implementation
}


void FrozenBoundary::applyBulkBC(VecField& Vi, GridLayout const& layout) const
{
    (void)Vi;
    (void)layout;
    // no implementation
}


void FrozenBoundary::applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                             LeavingParticles const& leavingParticles) const
{
    // TODO : need implementation here ?
}


void FrozenBoundary::applyIncomingParticleBC(BoundaryCondition const& temporaryBC, Pusher& pusher,
                                             GridLayout const& patchLayout,
                                             std::vector<Particle>& patchParticles, uint32 iesp)
{
    (void)temporaryBC;
    (void)pusher;
    (void)patchLayout;
    (void)patchParticles;
    (void)iesp;
    // no implementation
}
