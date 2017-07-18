#include "patchboundarycondition.h"

#include "BoundaryConditions/domainboundarycondition.h"

#include "pusher/pusherfactory.h"


#include "utilityphare.h"


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



void PatchBoundaryCondition::computePRAMoments(std::vector<uint32> const& orders)
{
    for (auto& boundary : boundaries_)
    {
        boundary->computePRAmoments(orders);
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
    for (auto& boundary : boundaries_)
    {
        boundary->applyDensityBC(N, patchLayout_);
    }
}


void PatchBoundaryCondition::applyBulkBC(VecField& Vi) const
{
    for (auto& boundary : boundaries_)
    {
        boundary->applyBulkBC(Vi, patchLayout_);
    }
}



void PatchBoundaryCondition::applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                                     LeavingParticles const& leavingParticles) const
{
    for (auto&& bc : boundaries_)
    {
        bc->applyOutgoingParticleBC(particleArray, leavingParticles);
    }
}



void PatchBoundaryCondition::applyIncomingParticleBC(Ions& ions, std::string const& pusherType,
                                                     double const& dt) const
{
    // Declare frozen BoundaryCondition
    // hard coded domain frozen boundary condition
    std::vector<DomainBoundaryCondition::BoundaryInfo> boundaries(2);

    // "first" is the edge coordinate
    boundaries[0].first = Edge::Xmin;
    boundaries[1].first = Edge::Xmax;

    // "second" is the type of boundary, here periodic
    boundaries[0].second = BoundaryType::Frozen;
    boundaries[1].second = BoundaryType::Frozen;


    for (auto&& bc : boundaries_)
    {
        // frozen boundary condition of the PRA
        // each PRA has its frozen boundaries built here
        std::unique_ptr<BoundaryCondition> temporaryBC{
            new DomainBoundaryCondition{bc->extendedLayout(), boundaries}};

        // Declare Pusher
        std::unique_ptr<Pusher> pusher{PusherFactory::createPusher(bc->layout(), pusherType, dt)};

        for (uint32 iesp = 0; iesp < ions.nbrSpecies(); ++iesp)
        {
            std::vector<Particle>& patchArray = ions.species(iesp).particles();

            bc->applyIncomingParticleBC(*temporaryBC, *pusher, patchLayout_, patchArray, iesp);
        }
    }
}
