
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
    // loop on dimensions of leavingParticles.particleIndicesAtMin/Max
    uint32 nbDims = leavingParticles.particleIndicesAtMax.size();

    for (uint32 dim = 0; dim < nbDims; ++dim)
    {
        std::vector<uint32> const& leavingAtMin = leavingParticles.particleIndicesAtMin[dim];
        std::vector<uint32> const& leavingAtMax = leavingParticles.particleIndicesAtMax[dim];

        std::vector<uint32> leavingIndexes;
        leavingIndexes.reserve(leavingAtMin.size() + leavingAtMax.size());

        leavingIndexes.insert(leavingIndexes.end(), leavingAtMin.begin(), leavingAtMin.end());
        leavingIndexes.insert(leavingIndexes.end(), leavingAtMax.begin(), leavingAtMax.end());

        removeParticles_(leavingIndexes, particleArray);
    }
}




/**
 * @brief PatchBoundary::applyIncomingParticleBC
 * Particles selected in member ions_ (of PatchBoundary) will be added
 * to particleArray table of the patch
 *
 * particleArray contains particles of a given species (of the patch)
 *
 * @param patchLayout
 * @param particleArray contains species particles of the patch
 *
 * @param iesp
 */
void PatchBoundary::applyIncomingParticleBC(BoundaryCondition const& temporaryBC, Pusher& pusher,
                                            GridLayout const& patchLayout,
                                            std::vector<Particle>& particleArray, uint32 iesp)
{
    std::vector<Particle>& PRAparticles = ions_.species(iesp).particles();

    // default initialization
    // std::vector<Particle> pushed_PRAparticles{PRAparticles};


    // TODO: define E
    VecField const& E = EMfields_.getE();

    // TODO: define B
    VecField const& B = EMfields_.getB();

    // TODO: define interpolator
    Interpolator interpolator(patchLayout.order());

    pusher.move(PRAparticles, PRAparticles, ions_.species(iesp).mass(), E, B, interpolator,
                temporaryBC);

    std::vector<uint32> leavingIndexes;
    leavingIndexes.reserve(PRAparticles.size());

    for (uint32 ipart = 0; ipart < PRAparticles.size(); ++ipart)
    {
        const Particle& part = PRAparticles[ipart];

        if (!isInSpecifiedBox(layout_, part, layout_))
        {
            if (isInSpecifiedBox(layout_, part, patchLayout))
            {
                Particle newPart;

                recomputeParticlePosistion_(layout_, patchLayout, part, newPart);

                particleArray.push_back(newPart);
            }

            // TODO: store index of the particle to be removed
            leavingIndexes.push_back(ipart);
        }
    }

    // TODO: remove particle from PRAparticles
    removeParticles_(leavingIndexes, PRAparticles);
}




/**
 * @brief PatchBoundary::removeParticles_
 * All particles indexed in leavingIndexes are removed from
 * particleArray
 *
 *
 * Successful test with Coliru
 */
void PatchBoundary::removeParticles_(std::vector<uint32> const& leavingIndexes,
                                     std::vector<Particle>& particleArray) const
{
    if (leavingIndexes.size() > 0)
    {
        std::vector<Particle> particleBuffer;

        particleBuffer.reserve(particleArray.size());
        particleBuffer.clear();

        uint32 iremove = 0;

        for (uint32 ipart = 0; ipart < particleArray.size(); ++ipart)
        {
            if (ipart != leavingIndexes[iremove])
            {
                particleBuffer.push_back(particleArray[ipart]);
            }

            if (ipart >= leavingIndexes[iremove])
            {
                if (iremove < leavingIndexes.size() - 1)
                {
                    iremove++;
                }
            }
        }

        std::swap(particleBuffer, particleArray);
    }
}



void PatchBoundary::recomputeParticlePosistion_(GridLayout const& praLayout,
                                                GridLayout const& patchLayout, Particle const& part,
                                                Particle& newPart) const
{
    newPart = part;

    int32 nbrGhostspra = static_cast<int32>(praLayout.nbrGhostNodes(QtyCentering::primal));

    // Components of the vector oriented
    // from the origin of the refined layout
    // to mother particle
    double compo_x
        = (praLayout.origin().x_ - patchLayout.origin().x_)
          + (static_cast<int32>(part.icell[0]) - nbrGhostspra + part.delta[0]) * praLayout.dx();

    double compo_y
        = (praLayout.origin().y_ - patchLayout.origin().y_)
          + (static_cast<int32>(part.icell[1]) - nbrGhostspra + part.delta[1]) * praLayout.dy();

    double compo_z
        = (praLayout.origin().z_ - patchLayout.origin().z_)
          + (static_cast<int32>(part.icell[2]) - nbrGhostspra + part.delta[2]) * praLayout.dz();

    int32 nbrGhostsPatch = static_cast<int32>(patchLayout.nbrGhostNodes(QtyCentering::primal));

    double compo_x_odx = compo_x / patchLayout.dx();
    double compo_y_ody = compo_y / patchLayout.dy();
    double compo_z_odz = compo_z / patchLayout.dz();

    switch (praLayout.nbDimensions())
    {
        case 3:
            newPart.icell[2]
                = static_cast<uint32>(nbrGhostsPatch + static_cast<int32>(std::floor(compo_z_odz)));

            newPart.delta[2] = static_cast<float>(compo_z_odz - std::floor(compo_z_odz));

        case 2:
            newPart.icell[1]
                = static_cast<uint32>(nbrGhostsPatch + static_cast<int32>(std::floor(compo_y_ody)));

            newPart.delta[1] = static_cast<float>(compo_y_ody - std::floor(compo_y_ody));

        case 1:
            newPart.icell[0]
                = static_cast<uint32>(nbrGhostsPatch + static_cast<int32>(std::floor(compo_x_odx)));

            newPart.delta[0] = static_cast<float>(compo_x_odx - std::floor(compo_x_odx));
            break;

        default: throw std::runtime_error("normalizeMotherPosition: wrong dimensionality");
    }
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
