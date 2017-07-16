#include "patchboundarycondition.h"



/**
 * @brief PatchBoundaryCondition::PatchBoundaryCondition
 * we initialize isPatchBC = true
 *
 *
 * @param refinedPRA
 * @param coarsePatch
 * @param coarseLayout
 * @param boundaries
 */
PatchBoundaryCondition::PatchBoundaryCondition(
    PRA const& refinedPRA, std::shared_ptr<Patch> coarsePatch, GridLayout const& coarseLayout,
    std::vector<std::unique_ptr<PatchBoundary>> boundaries)
    : refinedPRA_{refinedPRA}
    , parent_{coarsePatch}
    , coarseLayout_{coarseLayout}
    , boundaries_{std::move(boundaries)}
{
}


/**
 * @brief PatchBoundaryCondition::initializeGhostArea
 * In the case of a PatchBoundaryCondition this method
 * will initialize the PRA of the Patch.
 *
 *
 */
void PatchBoundaryCondition::initializeGhostArea()
{
    for (auto& boundary : boundaries_)
    {
        // effectively load the particles into Ions attribute of
        // PatchBoundary object using
        // the ParticleInitializer of each Species object stored
        // Ions instance
        boundary->initPRAParticles();

        // the electro-magnetic field has been
        // initialized by PatchBoundary constructor

        // the EM field will be further modified by
        // MLMD mecanisms
    }
}


void PatchBoundaryCondition::applyMagneticBC(VecField& B) const
{
}


void PatchBoundaryCondition::applyElectricBC(VecField& E) const
{
}


void PatchBoundaryCondition::applyCurrentBC(VecField& J) const
{
}


void PatchBoundaryCondition::applyDensityBC(Field& N) const
{
}


void PatchBoundaryCondition::applyBulkBC(VecField& Vi) const
{
}



void PatchBoundaryCondition::applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                                     LeavingParticles const& leavingParticles) const
{
}


void PatchBoundaryCondition::applyIncomingParticleBC(Ions& ions, std::string const& pusher,
                                                     double const& dt) const
{
}
