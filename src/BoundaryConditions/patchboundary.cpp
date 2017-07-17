
#include "BoundaryConditions/patchboundary.h"

#include "Interpolator/interpolator.h"
#include "Interpolator/particlemesh.h"

#include "AMR/MLMD/pra.h"
#include "AMR/particletests.h"




void PatchBoundary::initPRAParticles()
{
    // first, we clear particle information
    ions_.resetParticles();

    // we trigger actual loading of particles
    ions_.loadParticles();
}



void PatchBoundary::applyElectricBC(VecField& E, GridLayout const& layout) const
{
}


void PatchBoundary::applyMagneticBC(VecField& B, GridLayout const& layout) const
{
}



void PatchBoundary::applyCurrentBC(VecField& J, GridLayout const& layout) const
{
}



void PatchBoundary::applyDensityBC(Field& rhoPatch, GridLayout const& patchLayout) const
{
    Field const& rhoPRA = ions_.rho();

    switch (patchLayout.nbDimensions())
    {
        case 1: addPRAmomentsToPatch1D_(patchLayout, rhoPatch, rhoPRA, edge_); break;

        case 2: addPRAmomentsToPatch2D_(patchLayout, rhoPatch, rhoPRA, edge_); break;

        case 3: addPRAmomentsToPatch3D_(patchLayout, rhoPatch, rhoPRA, edge_); break;
    }
}


void PatchBoundary::applyBulkBC(VecField& bulkVelPatch, GridLayout const& patchLayout) const
{
    VecField const& bulkVelPRA = ions_.bulkVel();


    switch (patchLayout.nbDimensions())
    {
        case 1: addPRAmomentsToPatch1D_(patchLayout, bulkVelPatch, bulkVelPRA, edge_); break;

        case 2: addPRAmomentsToPatch2D_(patchLayout, bulkVelPatch, bulkVelPRA, edge_); break;

        case 3: addPRAmomentsToPatch3D_(patchLayout, bulkVelPatch, bulkVelPRA, edge_); break;
    }
}


void PatchBoundary::applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                            LeavingParticles const& leavingParticles) const
{
    removeOutgoingParticles_(particleArray, leavingParticles);
}


void PatchBoundary::removeOutgoingParticles_(std::vector<Particle>& particleArray,
                                             LeavingParticles const& leavingParticles) const
{
}


void PatchBoundary::applyIncomingParticleBC(BoundaryCondition const& temporaryBC, Pusher& pusher,
                                            GridLayout const& patchLayout,
                                            std::vector<Particle>& patchParticles, uint32 iesp)
{
}




/**
 * @brief PatchBoundary::computePRAmoments
 *
 */
void PatchBoundary::computePRAmoments(std::vector<uint32> const& orders)
{
    uint32 nbrSpecies = ions_.nbrSpecies();

    for (uint32 ispe = 0; ispe < nbrSpecies; ++ispe)
    {
        Species& species = ions_.species(ispe);

        Interpolator interpolator(orders[ispe]);

        computeChargeDensityAndFlux(interpolator, species, layout_, species.particles());
    }

    ions_.computeChargeDensity();
    ions_.computeBulkVelocity();
}




void PatchBoundary::addPRAmomentsToPatch1D_(GridLayout const& patchLayout, Field& rhoPatch,
                                            Field const& rhoPRA, Edge const& edge) const
{
    uint32 nbrNodes    = 0;
    uint32 iStartPatch = 0;
    uint32 iStartPRA   = 0;

    getPRAandPatchStartIndexes_(patchLayout, rhoPatch, rhoPRA, edge, Direction::X, nbrNodes,
                                iStartPatch, iStartPRA);

    for (uint32 iNode = 0; iNode < nbrNodes; ++iNode)
    {
        rhoPatch(iStartPatch + iNode) += rhoPRA(iStartPRA + iNode);
    }
}

void PatchBoundary::addPRAmomentsToPatch1D_(GridLayout const& patchLayout, VecField& bulkVelPatch,
                                            VecField const& bulkVelPRA, Edge const& edge) const
{
    uint32 nbrNodes    = 0;
    uint32 iStartPatch = 0;
    uint32 iStartPRA   = 0;

    for (uint32 iComp = 0; iComp < 3; ++iComp)
    {
        Field& fieldPatch     = bulkVelPatch.component(iComp);
        Field const& fieldPRA = bulkVelPRA.component(iComp);

        getPRAandPatchStartIndexes_(patchLayout, fieldPatch, fieldPRA, edge, Direction::X, nbrNodes,
                                    iStartPatch, iStartPRA);

        for (uint32 iNode = 0; iNode < nbrNodes; ++iNode)
        {
            fieldPatch(iStartPatch + iNode) += fieldPRA(iStartPRA + iNode);
        }
    }
}


void PatchBoundary::getPRAandPatchStartIndexes_(GridLayout const& patchLayout,
                                                Field const& fieldPatch, Field const& fieldPRA,
                                                Edge const& edge, Direction const& direction,
                                                uint32& nbrNodes, uint32& iStartPatch,
                                                uint32& iStartPRA) const
{
    // here we need the exact number of ghost nodes
    // no exception for 1st order
    uint32 nbrGhosts = static_cast<uint32>(std::floor(layout_.order() / 2.));

    nbrNodes = 2 * nbrGhosts + 1;

    // Default initialization Edge::Xmin
    iStartPatch = patchLayout.physicalStartIndex(fieldPatch, direction) - nbrGhosts;
    iStartPRA   = layout_.physicalEndIndex(fieldPRA, direction) - nbrGhosts;

    if (edge == Edge::Xmax)
    {
        iStartPatch = patchLayout.physicalEndIndex(fieldPatch, direction) - nbrGhosts;
        iStartPRA   = layout_.physicalStartIndex(fieldPRA, direction) - nbrGhosts;
    }
}



void PatchBoundary::addPRAmomentsToPatch2D_(GridLayout const& patchLayout, Field& rhoPatch,
                                            Field const& rhoPRA, Edge const& edge) const
{
    (void)patchLayout;
    (void)rhoPatch;
    (void)rhoPRA;
    (void)edge;
    throw std::runtime_error("addPRAmomentsToPatch2D_ : Not Implemented");
}

void PatchBoundary::addPRAmomentsToPatch2D_(GridLayout const& patchLayout, VecField& bulkVelPatch,
                                            VecField const& bulkVelPRA, Edge const& edge) const
{
    (void)patchLayout;
    (void)bulkVelPatch;
    (void)bulkVelPRA;
    (void)edge;
    throw std::runtime_error("addPRAmomentsToPatch2D_ : Not Implemented");
}


void PatchBoundary::addPRAmomentsToPatch3D_(GridLayout const& patchLayout, Field& rhoPatch,
                                            Field const& rhoPRA, Edge const& edge) const
{
    (void)patchLayout;
    (void)rhoPatch;
    (void)rhoPRA;
    (void)edge;
    throw std::runtime_error("addPRAmomentsToPatch3D_ : Not Implemented");
}

void PatchBoundary::addPRAmomentsToPatch3D_(GridLayout const& patchLayout, VecField& bulkVelPatch,
                                            VecField const& bulkVelPRA, Edge const& edge) const
{
    (void)patchLayout;
    (void)bulkVelPatch;
    (void)bulkVelPRA;
    (void)edge;
    throw std::runtime_error("addPRAmomentsToPatch3D_ : Not Implemented");
}
