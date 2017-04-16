#ifndef PATCHBOUNDARYCONDITION_H
#define PATCHBOUNDARYCONDITION_H

#include "AMR/patch.h"

#include "BoundaryConditions/boundary_conditions.h"
#include "BoundaryConditions/boundary.h"

#include "Initializer/initializerfactory.h"
#include "AMR/MLMD/pra.h"

#include <memory>




/**
 * @brief The PatchBoundaryCondition class
 *
 * PatchBoundaryCondition constructor basically needs:
 * + a PRA struct (containing innerBox and outerBox)
 * + a pointer to the coarse (parent) patch
 * + a copy of the coarse (parent) patch
 *
 *
 */
class PatchBoundaryCondition : public BoundaryCondition
{

private:
    PRA refinedPRA_;
    std::shared_ptr<Patch> parent_;
    GridLayout coarseLayout_;

    // these boundaries know what they are : patchboundary
    std::vector<std::unique_ptr<Boundary>> boundaries_;

public:
    PatchBoundaryCondition( PRA const & refinedPRA, std::shared_ptr<Patch> coarsePatch,
                            GridLayout const & coarseLayout,
                            std::vector<std::unique_ptr<Boundary>> boundaries )
        : refinedPRA_{refinedPRA}, parent_{coarsePatch},
          coarseLayout_{coarseLayout},
          boundaries_{std::move(boundaries)} {}

    virtual void applyMagneticBC(VecField& B) const override;
    virtual void applyElectricBC(VecField& E) const override;
    virtual void applyCurrentBC(VecField& J)  const override;
    virtual void applyDensityBC(Field& N)     const override;
    virtual void applyBulkBC(VecField& Vi)    const override;
    virtual void applyParticleBC(std::vector<Particle>& particleArray,
                                 LeavingParticles const& leavingParticles)  const override;

};

#endif // PATCHBOUNDARYCONDITION_H
