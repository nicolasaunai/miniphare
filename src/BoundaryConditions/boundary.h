#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "BoundaryConditions/boundary_conditions.h"
#include "Plasmas/particles.h"
#include "grid/gridlayout.h"
#include "leavingparticles.h"
#include "pusher/pusher.h"
#include "vecfield/vecfield.h"

/**
 * @brief defines a boundary. There are 2, 4 and 8 boundaries in 1D, 2D and 3D
 *
 * DomainBoundaryCondition and PatchBoundaryCondition have a collection of these
 * abstract Boundary objects. These boundaries can be periodic, conducting, etc.
 */
class Boundary
{
public:
    virtual void applyMagneticBC(VecField& B, GridLayout const& layout) const = 0;
    virtual void applyElectricBC(VecField& E, GridLayout const& layout) const = 0;
    virtual void applyCurrentBC(VecField& J, GridLayout const& layout) const  = 0;
    virtual void applyDensityBC(Field& J, GridLayout const& layout) const     = 0;
    virtual void applyBulkBC(VecField& Vi, GridLayout const& layout) const    = 0;
    virtual void applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                         LeavingParticles const& leavingParticles) const = 0;

    virtual void applyIncomingParticleBC(BoundaryCondition const& temporaryBC, Pusher& pusher,
                                         GridLayout const& patchLayout,
                                         std::vector<Particle>& patchParticles, uint32 iesp)
        = 0;
};

#endif // BOUNDARY_H
