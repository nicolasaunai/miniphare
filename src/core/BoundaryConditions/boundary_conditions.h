#ifndef FIELDBC3DCOLLECTION_H
#define FIELDBC3DCOLLECTION_H

#include <memory>
#include <vector>

#include "data/Field/field.h"
#include "data/Plasmas/ions.h"
#include "data/vecfield/vecfield.h"
#include "data/Plasmas/particles.h"
#include "leavingparticles.h"

enum class BoundaryType { Periodic, Frozen };

/**
 * @brief The BoundaryCondition class is the interface used in the Solver
 *
 * This class is an abstract interface used in the Solver. It can be either
 * a DomainBoundaryCondition or a PatchBoundaryCondition depending on whether
 * the current Patch is the root patch or a refined one.
 */
class BoundaryCondition
{
public:
    BoundaryCondition() {}

    virtual ~BoundaryCondition() = default;

    virtual void applyMagneticBC(VecField& B) const = 0;
    virtual void applyElectricBC(VecField& E) const = 0;
    virtual void applyCurrentBC(VecField& J) const  = 0;
    virtual void applyDensityBC(Field& N) const     = 0;
    virtual void applyFluxBC(Ions& ions) const      = 0;

    virtual void applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                         LeavingParticles const& leavingParticles)
        = 0;

    virtual void applyIncomingParticleBC(std::vector<Particle>& particles,
                                         std::string const& pusher, double const& dt,
                                         std::string const& species) const = 0;
};



#endif // FIELDBC3DCOLLECTION_H
