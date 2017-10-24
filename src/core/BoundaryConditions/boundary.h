#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "core/BoundaryConditions/boundary_conditions.h"
#include "core/pusher/pusher.h"
#include "data/Plasmas/particles.h"
#include "data/grid/gridlayout.h"
#include "data/vecfield/vecfield.h"
#include "leavingparticles.h"

/**
 * @brief defines a boundary. There are 2, 4 and 8 boundaries in 1D, 2D and 3D
 *
 * DomainBoundaryCondition and PatchBoundaryCondition have a collection of these
 * abstract Boundary objects. These boundaries can be periodic, conducting, etc.
 */
class Boundary
{
public:
    virtual void applyMagneticBC(VecField& B, GridLayout const& layout)       = 0;
    virtual void applyElectricBC(VecField& E, GridLayout const& layout) const = 0;
    virtual void applyCurrentBC(VecField& J, GridLayout const& layout) const  = 0;
    virtual void applyDensityBC(Field& J, GridLayout const& layout) const     = 0;
    virtual void applyFluxBC(Ions& ions, GridLayout const& layout) const      = 0;

    virtual void applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                         LeavingParticles const& leavingParticles) const = 0;

    virtual void applyIncomingParticleBC(BoundaryCondition& temporaryBC, Pusher& pusher,
                                         GridLayout const& patchLayout,
                                         std::vector<Particle>& patchParticles,
                                         std::string const& species)
        = 0;
};

#endif // BOUNDARY_H
