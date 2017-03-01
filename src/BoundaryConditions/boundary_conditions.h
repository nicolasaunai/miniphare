#ifndef FIELDBC3DCOLLECTION_H
#define FIELDBC3DCOLLECTION_H

#include <vector>
#include <memory>

#include "Field/field.h"
#include "vecfield/vecfield.h"
#include "Plasmas/particles.h"
#include "leavingparticles.h"

enum class BoundaryType{ Periodic };

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
    virtual void applyMagneticBC(VecField& B) const = 0;
    virtual void applyElectricBC(VecField& E) const = 0;
    virtual void applyCurrentBC(VecField& J)  const = 0;
    virtual void applyDensityBC(Field& N)     const = 0;
    virtual void applyBulkBC(VecField& Vi)    const = 0;
    virtual void applyParticleBC(std::vector<Particle>& particleArray,
                                 LeavingParticles const& leavingParticles) const = 0;

    virtual ~BoundaryCondition() = 0;
};



#endif // FIELDBC3DCOLLECTION_H
