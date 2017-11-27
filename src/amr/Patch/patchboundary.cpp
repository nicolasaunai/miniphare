
#include "amr/Patch/patchboundary.h"
#include "amr/MLMD/pra.h"
#include "amr/MLMD/praboundarycondition.h"
#include "amr/Refinement/coarsetorefinemesh.h"

#include "core/Interpolator/interpolator.h"
#include "core/Interpolator/particlemesh.h"

#include "data/vecfield/vecfieldoperations.h"

#include "utilities/particleutilities.h"

#include <iostream>



void PatchBoundary::initPRAParticles()
{
    ions_.resetParticles();
    ions_.loadParticles();
}



void PatchBoundary::applyElectricBC(VecField& E_patch, GridLayout const& patchLayout) const
{
    // Init E_interp on the PRA
    VecField E_interp = EMfields_.getE();

    interpolateElectricFieldInTime_(E_interp);

    switch (patchLayout.nbDimensions())
    {
        case 1: applyPRAfieldsToPatch1D_(patchLayout, E_patch, E_interp, edge_); break;

        case 2: applyPRAfieldsToPatch2D_(patchLayout, E_patch, E_interp, edge_); break;

        case 3: applyPRAfieldsToPatch3D_(patchLayout, E_patch, E_interp, edge_); break;
    }
}


void PatchBoundary::applyMagneticBC(VecField& B_patch, GridLayout const& patchLayout) const
{
    // Init B_interp on the PRA
    VecField B_interp = EMfields_.getB();

    interpolateMagneticFieldInTime_(B_interp);

    // we update Jtot on the PRA
    ampere_(B_interp, Jtot_);

    switch (patchLayout.nbDimensions())
    {
        case 1: applyPRAfieldsToPatch1D_(patchLayout, B_patch, B_interp, edge_); break;

        case 2: applyPRAfieldsToPatch2D_(patchLayout, B_patch, B_interp, edge_); break;

        case 3: applyPRAfieldsToPatch3D_(patchLayout, B_patch, B_interp, edge_); break;
    }
}



void PatchBoundary::applyCurrentBC(VecField& J_patch, GridLayout const& patchLayout) const
{
    switch (patchLayout.nbDimensions())
    {
        case 1: applyPRAfieldsToPatch1D_(patchLayout, J_patch, Jtot_, edge_); break;

        case 2: applyPRAfieldsToPatch2D_(patchLayout, J_patch, Jtot_, edge_); break;

        case 3: applyPRAfieldsToPatch3D_(patchLayout, J_patch, Jtot_, edge_); break;
    }
}


void PatchBoundary::interpolateElectricFieldInTime_(VecField& E_interp) const
{
    VecField const& Et1 = EMfields_.getE();
    VecField const& Et2 = correctedEMfields_.getE();

    timeInterpolation(Et1, Et2, E_interp, dtParent_, freeEvolutionTime_);
}


void PatchBoundary::interpolateMagneticFieldInTime_(VecField& B_interp) const
{
    VecField const& Bt1 = EMfields_.getB();
    VecField const& Bt2 = correctedEMfields_.getB();

    timeInterpolation(Bt1, Bt2, B_interp, dtParent_, freeEvolutionTime_);
}



void PatchBoundary::applyPRAfieldsToPatch1D_(GridLayout const& patchLayout, VecField& EMfieldPatch,
                                             VecField const& EMfieldPRA, Edge const& edge) const
{
    uint32 idirX = static_cast<uint32>(Direction::X);
    uint32 idirY = static_cast<uint32>(Direction::Y);
    uint32 idirZ = static_cast<uint32>(Direction::Z);

    Field& Fx = EMfieldPatch.component(idirX);
    Field& Fy = EMfieldPatch.component(idirY);
    Field& Fz = EMfieldPatch.component(idirZ);

    std::array<std::reference_wrapper<Field>, 3> FxyzPatch = {{Fx, Fy, Fz}};

    Field const& FxPRA = EMfieldPRA.component(idirX);
    Field const& FyPRA = EMfieldPRA.component(idirY);
    Field const& FzPRA = EMfieldPRA.component(idirZ);

    std::array<std::reference_wrapper<Field const>, 3> FxyzPRA = {{FxPRA, FyPRA, FzPRA}};

    for (uint32 ifield = 0; ifield < FxyzPatch.size(); ++ifield)
    {
        Field const& fieldPRA = FxyzPRA[ifield];
        Field& fieldPatch     = FxyzPatch[ifield];

        uint32 nbrNodes    = 0;
        uint32 iStartPatch = 0;
        uint32 iStartPRA   = 0;

        getPRAIndexesOverlappingPatchGhostNodes(patchLayout, fieldPatch, fieldPRA, edge,
                                                Direction::X, nbrNodes, iStartPatch, iStartPRA);

        for (uint32 iNode = 0; iNode < nbrNodes; ++iNode)
        {
            fieldPatch(iStartPatch + iNode) = fieldPRA(iStartPRA + iNode);
        }
    }
}


void PatchBoundary::getPRAIndexesOverlappingPatchGhostNodes(
    GridLayout const& patchLayout, Field const& fieldPatch, Field const& fieldPRA, Edge const& edge,
    Direction const& direction, uint32& nbrNodes, uint32& iStartPatch, uint32& iStartPRA) const
{
    uint32 nbrGhosts = layout_.nbrGhostNodes(fieldPRA, direction);

    bool isDual = (layout_.fieldCentering(fieldPRA, direction) == QtyCentering::dual);

    nbrNodes = nbrGhosts;

    if (edge == Edge::Xmin)
    {
        /*  || denotes the boundary */
        /*  x  denotes a dual node  */
        /*  $  denotes arrow        */
        /* Example with 4th order: 2 ghost nodes */
        /*                        >|< primal physicalEndIndex of PRA */
        /* PRA      <--|--x--|--x--||--x--|--x--|       */
        /*             $     $                          */
        /*             $     $                          */
        /* Patch       |--x--|--x--||--x--|--x--|-->    */
        iStartPatch = patchLayout.ghostStartIndex(fieldPatch, direction);
        iStartPRA   = layout_.physicalEndIndex(fieldPRA, direction) - nbrGhosts;

        /* Example with 4th order: 2 ghost nodes */
        /*                     >|< dual physicalEndIndex of PRA */
        /* PRA      <--|--x--|--x--||--x--|--x--|    */
        /*                $     $                    */
        /*                $     $                    */
        /* Patch       |--x--|--x--||--x--|--x--|--> */
        if (isDual)
            iStartPRA += 1;
    }
    else if (edge == Edge::Xmax)
    {
        /*  || denotes the boundary */
        /*  x  denotes a dual node  */
        /*  $  denotes arrow        */
        /* Example with 4th order: 2 ghost nodes */
        /*                        >|< primal physicalStartIndex of PRA */
        /* PRA         |--x--|--x--||--x--|--x--|-->      */
        /*                                $     $         */
        /*                                $     $         */
        /* Patch    <--|--x--|--x--||--x--|--x--|         */
        iStartPatch = patchLayout.physicalEndIndex(fieldPatch, direction) + 1;
        iStartPRA   = layout_.physicalStartIndex(fieldPRA, direction) + 1;

        /*  || denotes the boundary */
        /*  x  denotes a dual node  */
        /*  $  denotes arrow        */
        /* Example with 4th order: 2 ghost nodes */
        /*                        >|< dual physicalStartIndex of PRA */
        /* PRA         |--x--|--x--||--x--|--x--|-->      */
        /*                             $     $            */
        /*                             $     $            */
        /* Patch    <--|--x--|--x--||--x--|--x--|         */
        if (isDual)
            iStartPRA -= 1;
    }
}



void PatchBoundary::applyPRAfieldsToPatch2D_(GridLayout const& patchLayout, VecField& EMfieldPatch,
                                             VecField const& EMfieldPRA, Edge const& edge) const
{
    (void)patchLayout;
    (void)EMfieldPatch;
    (void)EMfieldPRA;
    (void)edge;
    throw std::runtime_error("applyPRAfieldsToPatch2D_ : Not Implemented");
}


void PatchBoundary::applyPRAfieldsToPatch3D_(GridLayout const& patchLayout, VecField& EMfieldPatch,
                                             VecField const& EMfieldPRA, Edge const& edge) const
{
    (void)patchLayout;
    (void)EMfieldPatch;
    (void)EMfieldPRA;
    (void)edge;
    throw std::runtime_error("applyPRAfieldsToPatch3D_ : Not Implemented");
}




void PatchBoundary::applyDensityBC(Field& rhoPatch, GridLayout const& patchLayout) const
{
    Field const& rhoPRA = ions_.rho();

    switch (patchLayout.nbDimensions())
    {
        case 1: addPRAChargeDensityToPatch1D_(patchLayout, rhoPatch, rhoPRA, edge_); break;
        case 2: addPRAChargeDensityToPatch2D_(patchLayout, rhoPatch, rhoPRA, edge_); break;
        case 3: addPRAChargeDensityToPatch3D_(patchLayout, rhoPatch, rhoPRA, edge_); break;
    }
}



void PatchBoundary::applyFluxBC(Ions& ionsPatch, GridLayout const& patchLayout) const
{
    switch (patchLayout.nbDimensions())
    {
        case 1: addPRAFluxesToPatch1D_(patchLayout, ionsPatch, ions_, edge_); break;
        case 2: addPRAFluxesToPatch2D_(patchLayout, ionsPatch, ions_, edge_); break;
        case 3: addPRAFluxesToPatch3D_(patchLayout, ionsPatch, ions_, edge_); break;
    }
}


void PatchBoundary::applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                            LeavingParticles const& leavingParticles) const
{
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
void PatchBoundary::applyIncomingParticleBC(BoundaryCondition& temporaryBC, Pusher& pusher,
                                            GridLayout const& patchLayout,
                                            std::vector<Particle>& particleArray,
                                            std::string const& species)
{
    uint32 iesp = ions_.speciesID(species);

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
    try
    {
        PRABoundaryCondition& boundaryCond = dynamic_cast<PRABoundaryCondition&>(temporaryBC);

        std::vector<Particle>& incomingBucket{boundaryCond.incomingBucket()};

        for (Particle const& part : incomingBucket)
        {
            particleArray.push_back(part);
        }

        // we have to reset incoming particle bucket
        // before considering the next species
        boundaryCond.resetBucket();
    }
    catch (std::bad_cast excep)
    {
        std::cout << "PatchBoundary::applyIncomingParticleBC => Caught bad cast\n";
    }
}




/**
 * @brief PatchBoundary::computePRAmoments
 *
 */
void PatchBoundary::computePRADensityAndFlux(uint32 order)
{
    uint32 nbrSpecies = ions_.nbrSpecies();

    for (uint32 ispe = 0; ispe < nbrSpecies; ++ispe)
    {
        Species& species = ions_.species(ispe);

        Interpolator interpolator(order);

        computeChargeDensityAndFlux(interpolator, species, layout_, species.particles());
    }
}



void PatchBoundary::computePRAChargeDensity()
{
    ions_.computeChargeDensity();
}


void PatchBoundary::addPRAChargeDensityToPatch1D_(GridLayout const& patchLayout, Field& rhoPatch,
                                                  Field const& rhoPRA, Edge const& edge) const
{
    uint32 nbrNodes    = 0;
    uint32 iStartPatch = 0;
    uint32 iStartPRA   = 0;

    getPRAandPatchStartIndexes_(patchLayout, rhoPatch, rhoPRA, edge, Direction::X, nbrNodes,
                                iStartPatch, iStartPRA);

    // HERE we add !
    for (uint32 iNode = 0; iNode < nbrNodes; ++iNode)
    {
        rhoPatch(iStartPatch + iNode) += rhoPRA(iStartPRA + iNode);
    }
}


void PatchBoundary::addPRAFluxesToPatch1D_(GridLayout const& patchLayout, Ions& ionsPatch,
                                           Ions const& ionsPRA, Edge const& edge) const
{
    uint32 iCompX = static_cast<uint32>(Direction::X);
    uint32 iCompY = static_cast<uint32>(Direction::Y);
    uint32 iCompZ = static_cast<uint32>(Direction::Z);

    for (uint32 ispe = 0; ispe < ionsPatch.nbrSpecies(); ++ispe)
    {
        uint32 nbrNodes    = 0;
        uint32 iStartPatch = 0;
        uint32 iStartPRA   = 0;

        Species& speciesPatch     = ionsPatch.species(ispe);
        Species const& speciesPRA = ionsPRA.species(ispe);

        std::array<std::reference_wrapper<Field>, 3> fxyzPatch
            = {{speciesPatch.flux(iCompX), speciesPatch.flux(iCompY), speciesPatch.flux(iCompZ)}};

        std::array<std::reference_wrapper<Field const>, 3> fxyzPRA
            = {{speciesPRA.flux(iCompX), speciesPRA.flux(iCompY), speciesPRA.flux(iCompZ)}};

        for (uint32 ifield = 0; ifield < fxyzPatch.size(); ++ifield)
        {
            Field& fluxPatch     = fxyzPatch[ifield];
            Field const& fluxPRA = fxyzPRA[ifield];

            getPRAandPatchStartIndexes_(patchLayout, fluxPatch, fluxPRA, edge, Direction::X,
                                        nbrNodes, iStartPatch, iStartPRA);

            for (uint32 iNode = 0; iNode < nbrNodes; ++iNode)
                fluxPatch(iStartPatch + iNode) += fluxPRA(iStartPRA + iNode);
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



void PatchBoundary::addPRAChargeDensityToPatch2D_(GridLayout const& patchLayout, Field& rhoPatch,
                                                  Field const& rhoPRA, Edge const& edge) const
{
    (void)patchLayout;
    (void)rhoPatch;
    (void)rhoPRA;
    (void)edge;
    throw std::runtime_error("addPRAChargeDensityToPatch2D_ : Not Implemented");
}


void PatchBoundary::addPRAFluxesToPatch2D_(GridLayout const& patchLayout, Ions& ionsPatch,
                                           Ions const& ionsPRA, Edge const& edge) const
{
    (void)patchLayout;
    (void)ionsPatch;
    (void)ionsPRA;
    (void)edge;
    throw std::runtime_error("addPRAFluxesToPatch2D_ : Not Implemented");
}



void PatchBoundary::addPRAChargeDensityToPatch3D_(GridLayout const& patchLayout, Field& rhoPatch,
                                                  Field const& rhoPRA, Edge const& edge) const
{
    (void)patchLayout;
    (void)rhoPatch;
    (void)rhoPRA;
    (void)edge;
    throw std::runtime_error("addPRAChargeDensityToPatch3D_ : Not Implemented");
}


void PatchBoundary::addPRAFluxesToPatch3D_(GridLayout const& patchLayout, Ions& ionsPatch,
                                           Ions const& ionsPRA, Edge const& edge) const
{
    (void)patchLayout;
    (void)ionsPatch;
    (void)ionsPRA;
    (void)edge;
    throw std::runtime_error("addPRAFluxesToPatch3D_ : Not Implemented");
}



void PatchBoundary::updateCorrectedEMfields(GridLayout const& parentLayout,
                                            Electromag const& parentElectromag)
{
    // We update the electromagnetic field on the PRA layout
    // PRA == PatchBoundary, we just use the private layout_
    ElectromagInitializer emInitializer{layout_, "_EMField", "_EMFields"};

    // A linear interpolator is enough here (= 1)
    Interpolator interpolator(1);

    // Now we compute the E and B fields
    // of the ElectromagInitializer
    fieldAtRefinedNodes(interpolator, parentLayout, parentElectromag, layout_, emInitializer);

    // We now update the correctedEM field of the PRA
    correctedEMfields_.setFields(emInitializer);
}



void PatchBoundary::updateEMfields()
{
    EMfields_ = correctedEMfields_;
}


void PatchBoundary::resetFreeEvolutionTime()
{
    freeEvolutionTime_ = 0.;
    std::cout << "RESET, Free evolution time = " << freeEvolutionTime_ << std::endl;
}


void PatchBoundary::updateFreeEvolutionTime(double dt)
{
    std::cout << "Free evolution time = " << freeEvolutionTime_ << std::endl;
    freeEvolutionTime_ += dt;
    std::cout << "Free evolution time = " << freeEvolutionTime_ << std::endl;
}
