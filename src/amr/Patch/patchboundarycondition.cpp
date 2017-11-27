#include "amr/Patch/patchboundarycondition.h"

#include "amr/MLMD/praboundarycondition.h"
#include "core/pusher/pusherfactory.h"


#include "utilities/box.h"


#include <algorithm>

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
    PRA const& refinedPRA, std::shared_ptr<Patch> coarsePatch, GridLayout const& refinedLayout,
    std::vector<std::unique_ptr<PatchBoundary>> boundaries)
    : refinedPRA_{refinedPRA}
    , parent_{coarsePatch}
    , patchLayout_{refinedLayout}
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
void PatchBoundaryCondition::initializePRAparticles()
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



void PatchBoundaryCondition::computePRADensityAndFlux(uint32 order)
{
    for (auto& boundary : boundaries_)
    {
        boundary->computePRADensityAndFlux(order);
    }
}


void PatchBoundaryCondition::computePRAChargeDensity()
{
    for (auto& boundary : boundaries_)
    {
        boundary->computePRAChargeDensity();
    }
}


void PatchBoundaryCondition::updateCorrectedEMfields(GridLayout const& parentLayout,
                                                     Electromag const& parentElectromag)
{
    for (auto& boundary : boundaries_)
    {
        boundary->updateCorrectedEMfields(parentLayout, parentElectromag);
    }
}


void PatchBoundaryCondition::updateEMfields()
{
    for (auto& boundary : boundaries_)
    {
        boundary->updateEMfields();
    }
}


void PatchBoundaryCondition::applyElectricBC(VecField& E) const
{
    for (auto& boundary : boundaries_)
    {
        boundary->applyElectricBC(E, patchLayout_);
    }
}


void PatchBoundaryCondition::applyMagneticBC(VecField& B) const
{
    for (auto& boundary : boundaries_)
    {
        boundary->applyMagneticBC(B, patchLayout_);
    }
}


void PatchBoundaryCondition::applyCurrentBC(VecField& J) const
{
    for (auto& boundary : boundaries_)
    {
        boundary->applyCurrentBC(J, patchLayout_);
    }
}



void PatchBoundaryCondition::applyDensityBC(Field& N) const
{
    for (auto& boundary : boundaries_)
    {
        boundary->applyDensityBC(N, patchLayout_);
    }
}



void PatchBoundaryCondition::applyFluxBC(Ions& ions) const
{
    for (auto& boundary : boundaries_)
    {
        boundary->applyFluxBC(ions, patchLayout_);
    }
}


void PatchBoundaryCondition::applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                                     LeavingParticles const& leavingParticles)
{
    removeOutgoingParticles_(particleArray, leavingParticles);
}


void PatchBoundaryCondition::removeOutgoingParticles_(
    std::vector<Particle>& particleArray, LeavingParticles const& leavingParticles) const
{
    // loop on dimensions of leavingParticles.particleIndicesAtMin/Max
    uint32 nbDims = static_cast<uint32>(leavingParticles.particleIndicesAtMax.size());
    std::vector<uint32> leavingIndexes;

    // we need to concatenate all leaving particles to remove them all at once
    // if we don't then leaving indexes won't match leaving particles in the
    // particle array any more since the remove() operation shuffles the indexes.
    for (uint32 dim = 0; dim < nbDims; ++dim)
    {
        std::vector<int32> const& leavingAtMin = leavingParticles.particleIndicesAtMin[dim];
        std::vector<int32> const& leavingAtMax = leavingParticles.particleIndicesAtMax[dim];

        leavingIndexes.insert(leavingIndexes.end(), leavingAtMin.begin(), leavingAtMin.end());
        leavingIndexes.insert(leavingIndexes.end(), leavingAtMax.begin(), leavingAtMax.end());
    }

    // the index array should be sorted
    std::sort(leavingIndexes.begin(), leavingIndexes.end());

    // in case a particle leaves at more than 1 boundary, e.g. x AND y
    // its index will be found several times in the concatenated array
    // so call unique() will remove doubles.
    std::unique(leavingIndexes.begin(), leavingIndexes.end());

    // ok ready to remove particles now.
    removeParticles(leavingIndexes, particleArray);
}


void PatchBoundaryCondition::applyIncomingParticleBC(std::vector<Particle>& patchArray,
                                                     std::string const& pusherType,
                                                     double const& dt,
                                                     std::string const& species) const
{
    for (auto&& bc : boundaries_)
    {
        // frozen boundary condition of the PRA
        // each PRA has its frozen boundaries built here
        // std::unique_ptr<BoundaryCondition> temporaryBC{
        //    new DomainBoundaryCondition{bc->extendedLayout(), boundaries}};

        std::unique_ptr<BoundaryCondition> temporaryBC{
            new PRABoundaryCondition{patchLayout_, bc->layout()}};

        // Declare Pusher
        std::unique_ptr<Pusher> pusher{PusherFactory::createPusher(bc->layout(), pusherType, dt)};

        bc->applyIncomingParticleBC(*temporaryBC, *pusher, patchLayout_, patchArray, species);
    }
}



void PatchBoundaryCondition::resetFreeEvolutionTime()
{
    for (auto& boundary : boundaries_)
    {
        boundary->resetFreeEvolutionTime();
    }
}



void PatchBoundaryCondition::updateFreeEvolutionTime(double dt)
{
    for (auto& boundary : boundaries_)
    {
        boundary->updateFreeEvolutionTime(dt);
    }
}
