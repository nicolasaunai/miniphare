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
        PRA const & refinedPRA, std::shared_ptr<Patch> coarsePatch,
        GridLayout const & coarseLayout,
        std::vector<std::unique_ptr<PatchBoundary>> boundaries )
    : BoundaryCondition( true ),
      refinedPRA_{refinedPRA}, parent_{coarsePatch},
      coarseLayout_{coarseLayout},
      boundaries_{std::move(boundaries)}
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
    for( auto & boundary: boundaries_ )
    {
        boundary->initPRAParticles() ;
    }

}


void PatchBoundaryCondition::applyMagneticBC(VecField& B) const
{

}


void PatchBoundaryCondition::applyElectricBC(VecField& E) const
{

}


void PatchBoundaryCondition::applyCurrentBC(VecField& J)  const
{

}


void PatchBoundaryCondition::applyDensityBC(Field& N)     const
{

}


void PatchBoundaryCondition::applyBulkBC(VecField& Vi)    const
{

}



void PatchBoundaryCondition::applyParticleBC(std::vector<Particle>& particleArray,
                             LeavingParticles const& leavingParticles)  const
{

}


