#ifndef PATCHBOUNDARYCONDITION_H
#define PATCHBOUNDARYCONDITION_H

#include "amr/MLMD/gca.h"
#include "amr/Patch/patch.h"

#include "amr/Patch/patchboundary.h"
#include "core/BoundaryConditions/boundary.h"
#include "core/BoundaryConditions/boundary_conditions.h"
#include "initializer/initializerfactory.h"




#include <memory>




/**
 * @brief The PatchBoundaryCondition class
 *
 * PatchBoundaryCondition constructor basically needs:
 * + a GCA struct (containing innerBox and outerBox)
 * + a pointer to the coarse (parent) patch
 * + a copy of the coarse (parent) patch layout
 *
 *
 */
class PatchBoundaryCondition : public BoundaryCondition
{
private:
    GCA refinedGCA_;
    std::shared_ptr<Patch> parent_;
    GridLayout patchLayout_;

    // We know we are dealing with PatchBoundary objects
    std::vector<std::unique_ptr<PatchBoundary>> boundaries_;

    void removeOutgoingParticles_(std::vector<Particle>& particleArray,
                                  LeavingParticles const& leavingParticles) const;

public:
    PatchBoundaryCondition(GCA const& refinedGCA, std::shared_ptr<Patch> coarsePatch,
                           GridLayout const& coarseLayout,
                           std::vector<std::unique_ptr<PatchBoundary>> boundaries);

    virtual void applyMagneticBC(VecField& B) const override;
    virtual void applyElectricBC(VecField& E) const override;
    virtual void applyCurrentBC(VecField& J) const override;
    virtual void applyDensityBC(Field& N) const override;
    virtual void applyFluxBC(Ions& ions) const override;

    virtual void applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                         LeavingParticles const& leavingParticles) override;

    virtual void applyIncomingParticleBC(std::vector<Particle>& patchArray,
                                         std::string const& pusherType, double const& dt,
                                         std::string const& species, bool update) const override;

    void initializeGCAparticles();

    void computeGCADensityAndFlux(uint32 order);
    void computeGCAChargeDensity();

    void updateCorrectedEMfields(GridLayout const& parentLayout,
                                 Electromag const& parentElectromag);

    void updateEMfields();

    void resetFreeEvolutionTime();
    void updateFreeEvolutionTime(double dt);
};

#endif // PATCHBOUNDARYCONDITION_H
