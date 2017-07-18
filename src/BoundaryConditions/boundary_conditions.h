#ifndef FIELDBC3DCOLLECTION_H
#define FIELDBC3DCOLLECTION_H

#include <memory>
#include <vector>

#include "Field/field.h"
#include "Plasmas/ions.h"
#include "Plasmas/particles.h"
#include "leavingparticles.h"
#include "vecfield/vecfield.h"

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
    virtual void applyBulkBC(VecField& Vi) const    = 0;
    virtual void applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                         LeavingParticles const& leavingParticles) const = 0;
    virtual void applyIncomingParticleBC(Ions& ions, std::string const& pusher,
                                         double const& dt) const = 0;
};



#endif // FIELDBC3DCOLLECTION_H
