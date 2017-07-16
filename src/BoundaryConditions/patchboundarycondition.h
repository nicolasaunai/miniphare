#ifndef PATCHBOUNDARYCONDITION_H
#define PATCHBOUNDARYCONDITION_H

#include "AMR/patch.h"

#include "BoundaryConditions/boundary.h"
#include "BoundaryConditions/boundary_conditions.h"
#include "BoundaryConditions/patchboundary.h"

#include "AMR/MLMD/pra.h"
#include "Initializer/initializerfactory.h"

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

    // We know we are dealing with PatchBoundary objects
    std::vector<std::unique_ptr<PatchBoundary>> boundaries_;

public:
    PatchBoundaryCondition(PRA const& refinedPRA, std::shared_ptr<Patch> coarsePatch,
                           GridLayout const& coarseLayout,
                           std::vector<std::unique_ptr<PatchBoundary>> boundaries);

    virtual void applyMagneticBC(VecField& B) const override;
    virtual void applyElectricBC(VecField& E) const override;
    virtual void applyCurrentBC(VecField& J) const override;
    virtual void applyDensityBC(Field& N) const override;
    virtual void applyBulkBC(VecField& Vi) const override;

    virtual void applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                         LeavingParticles const& leavingParticles) const override;

    virtual void applyIncomingParticleBC(Ions& ions, std::string const& pusher,
                                         double const& dt) const override;

    virtual bool hasARepopulationArea() const override { return true; }

    virtual void initializeGhostArea() override;
};

#endif // PATCHBOUNDARYCONDITION_H
