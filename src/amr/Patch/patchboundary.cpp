
#include "amr/Patch/patchboundary.h"
#include "amr/MLMD/gca.h"
#include "amr/MLMD/gcaboundarycondition.h"
#include "amr/Refinement/coarsetorefinemesh.h"

#include "core/Interpolator/interpolator.h"
#include "core/Interpolator/particlemesh.h"

#include "data/vecfield/vecfieldoperations.h"

#include "utilities/particleutilities.h"

#include <iostream>



void PatchBoundary::initGCAParticles()
{
    ions_.resetParticles();
    ions_.loadParticles();
}



void PatchBoundary::applyElectricBC(VecField& E_patch, GridLayout const& patchLayout) const
{
    // Init E_interp on the GCA
    VecField E_interp = EMfields_.getE();

    interpolateElectricFieldInTime_(E_interp);

    switch (patchLayout.nbDimensions())
    {
        case 1: applyGCAfieldsToPatch1D_(patchLayout, E_patch, E_interp, edge_); break;

        case 2: applyGCAfieldsToPatch2D_(patchLayout, E_patch, E_interp, edge_); break;

        case 3: applyGCAfieldsToPatch3D_(patchLayout, E_patch, E_interp, edge_); break;
    }
}


void PatchBoundary::applyMagneticBC(VecField& B_patch, GridLayout const& patchLayout) const
{
    // Init B_interp on the GCA
    VecField B_interp = EMfields_.getB();

    interpolateMagneticFieldInTime_(B_interp);

    // we update Jtot on the GCA
    ampere_(B_interp, Jtot_);

    switch (patchLayout.nbDimensions())
    {
        case 1: applyGCAfieldsToPatch1D_(patchLayout, B_patch, B_interp, edge_); break;

        case 2: applyGCAfieldsToPatch2D_(patchLayout, B_patch, B_interp, edge_); break;

        case 3: applyGCAfieldsToPatch3D_(patchLayout, B_patch, B_interp, edge_); break;
    }
}



void PatchBoundary::applyCurrentBC(VecField& J_patch, GridLayout const& patchLayout) const
{
    switch (patchLayout.nbDimensions())
    {
        case 1: applyGCAfieldsToPatch1D_(patchLayout, J_patch, Jtot_, edge_); break;

        case 2: applyGCAfieldsToPatch2D_(patchLayout, J_patch, Jtot_, edge_); break;

        case 3: applyGCAfieldsToPatch3D_(patchLayout, J_patch, Jtot_, edge_); break;
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



void PatchBoundary::applyGCAfieldsToPatch1D_(GridLayout const& patchLayout, VecField& EMfieldPatch,
                                             VecField const& EMfieldGCA, Edge const& edge) const
{
    uint32 idirX = static_cast<uint32>(Direction::X);
    uint32 idirY = static_cast<uint32>(Direction::Y);
    uint32 idirZ = static_cast<uint32>(Direction::Z);

    Field& Fx = EMfieldPatch.component(idirX);
    Field& Fy = EMfieldPatch.component(idirY);
    Field& Fz = EMfieldPatch.component(idirZ);

    std::array<std::reference_wrapper<Field>, 3> FxyzPatch = {{Fx, Fy, Fz}};

    Field const& FxGCA = EMfieldGCA.component(idirX);
    Field const& FyGCA = EMfieldGCA.component(idirY);
    Field const& FzGCA = EMfieldGCA.component(idirZ);

    std::array<std::reference_wrapper<Field const>, 3> FxyzGCA = {{FxGCA, FyGCA, FzGCA}};

    for (uint32 ifield = 0; ifield < FxyzPatch.size(); ++ifield)
    {
        Field const& fieldGCA = FxyzGCA[ifield];
        Field& fieldPatch     = FxyzPatch[ifield];

        uint32 nbrNodes    = 0;
        uint32 iStartPatch = 0;
        uint32 iStartGCA   = 0;

        getGCAIndexesOverlappingPatchGhostNodes(patchLayout, fieldPatch, fieldGCA, edge,
                                                Direction::X, nbrNodes, iStartPatch, iStartGCA);

        for (uint32 iNode = 0; iNode < nbrNodes; ++iNode)
        {
            fieldPatch(iStartPatch + iNode) = fieldGCA(iStartGCA + iNode);
        }
    }
}


void PatchBoundary::getGCAIndexesOverlappingPatchGhostNodes(
    GridLayout const& patchLayout, Field const& fieldPatch, Field const& fieldGCA, Edge const& edge,
    Direction const& direction, uint32& nbrNodes, uint32& iStartPatch, uint32& iStartGCA) const
{
    uint32 nbrGhosts = layout_.nbrGhostNodes(fieldGCA, direction);

    bool isDual = (layout_.fieldCentering(fieldGCA, direction) == QtyCentering::dual);

    nbrNodes = nbrGhosts;

    if (edge == Edge::Xmin)
    {
        /*  || denotes the boundary */
        /*  x  denotes a dual node  */
        /*  $  denotes arrow        */
        /* Example with 4th order: 2 ghost nodes */
        /*                        >|< primal physicalEndIndex of GCA */
        /* GCA      <--|--x--|--x--||--x--|--x--|       */
        /*             $     $                          */
        /*             $     $                          */
        /* Patch       |--x--|--x--||--x--|--x--|-->    */
        iStartPatch = patchLayout.ghostStartIndex(fieldPatch, direction);
        iStartGCA   = layout_.physicalEndIndex(fieldGCA, direction) - nbrGhosts;

        /* Example with 4th order: 2 ghost nodes */
        /*                     >|< dual physicalEndIndex of GCA */
        /* GCA      <--|--x--|--x--||--x--|--x--|    */
        /*                $     $                    */
        /*                $     $                    */
        /* Patch       |--x--|--x--||--x--|--x--|--> */
        if (isDual)
            iStartGCA += 1;
    }
    else if (edge == Edge::Xmax)
    {
        /*  || denotes the boundary */
        /*  x  denotes a dual node  */
        /*  $  denotes arrow        */
        /* Example with 4th order: 2 ghost nodes */
        /*                        >|< primal physicalStartIndex of GCA */
        /* GCA         |--x--|--x--||--x--|--x--|-->      */
        /*                                $     $         */
        /*                                $     $         */
        /* Patch    <--|--x--|--x--||--x--|--x--|         */
        iStartPatch = patchLayout.physicalEndIndex(fieldPatch, direction) + 1;
        iStartGCA   = layout_.physicalStartIndex(fieldGCA, direction) + 1;

        /*  || denotes the boundary */
        /*  x  denotes a dual node  */
        /*  $  denotes arrow        */
        /* Example with 4th order: 2 ghost nodes */
        /*                        >|< dual physicalStartIndex of GCA */
        /* GCA         |--x--|--x--||--x--|--x--|-->      */
        /*                             $     $            */
        /*                             $     $            */
        /* Patch    <--|--x--|--x--||--x--|--x--|         */
        if (isDual)
            iStartGCA -= 1;
    }
}



void PatchBoundary::applyGCAfieldsToPatch2D_(GridLayout const& patchLayout, VecField& EMfieldPatch,
                                             VecField const& EMfieldGCA, Edge const& edge) const
{
    (void)patchLayout;
    (void)EMfieldPatch;
    (void)EMfieldGCA;
    (void)edge;
    throw std::runtime_error("applyGCAfieldsToPatch2D_ : Not Implemented");
}


void PatchBoundary::applyGCAfieldsToPatch3D_(GridLayout const& patchLayout, VecField& EMfieldPatch,
                                             VecField const& EMfieldGCA, Edge const& edge) const
{
    (void)patchLayout;
    (void)EMfieldPatch;
    (void)EMfieldGCA;
    (void)edge;
    throw std::runtime_error("applyGCAfieldsToPatch3D_ : Not Implemented");
}




void PatchBoundary::applyDensityBC(Field& rhoPatch, GridLayout const& patchLayout) const
{
    Field const& rhoGCA = ions_.rho();

    switch (patchLayout.nbDimensions())
    {
        case 1: addGCAChargeDensityToPatch1D_(patchLayout, rhoPatch, rhoGCA, edge_); break;
        case 2: addGCAChargeDensityToPatch2D_(patchLayout, rhoPatch, rhoGCA, edge_); break;
        case 3: addGCAChargeDensityToPatch3D_(patchLayout, rhoPatch, rhoGCA, edge_); break;
    }
}



void PatchBoundary::applyFluxBC(Ions& ionsPatch, GridLayout const& patchLayout) const
{
    switch (patchLayout.nbDimensions())
    {
        case 1: addGCAFluxesToPatch1D_(patchLayout, ionsPatch, ions_, edge_); break;
        case 2: addGCAFluxesToPatch2D_(patchLayout, ionsPatch, ions_, edge_); break;
        case 3: addGCAFluxesToPatch3D_(patchLayout, ionsPatch, ions_, edge_); break;
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

    std::vector<Particle>& GCAparticles = ions_.species(iesp).particles();

    // default initialization
    // std::vector<Particle> pushed_GCAparticles{GCAparticles};


    // TODO: define E
    VecField const& E = EMfields_.getE();

    // TODO: define B
    VecField const& B = EMfields_.getB();

    // TODO: define interpolator
    Interpolator interpolator(patchLayout.order());

    pusher.move(GCAparticles, GCAparticles, ions_.species(iesp).mass(), E, B, interpolator,
                temporaryBC);
    try
    {
        GCABoundaryCondition& boundaryCond = dynamic_cast<GCABoundaryCondition&>(temporaryBC);

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
 * @brief PatchBoundary::computeGCAmoments
 *
 */
void PatchBoundary::computeGCADensityAndFlux(uint32 order)
{
    uint32 nbrSpecies = ions_.nbrSpecies();

    for (uint32 ispe = 0; ispe < nbrSpecies; ++ispe)
    {
        Species& species = ions_.species(ispe);

        Interpolator interpolator(order);

        computeChargeDensityAndFlux(interpolator, species, layout_, species.particles());
    }
}



void PatchBoundary::computeGCAChargeDensity()
{
    ions_.computeChargeDensity();
}


void PatchBoundary::addGCAChargeDensityToPatch1D_(GridLayout const& patchLayout, Field& rhoPatch,
                                                  Field const& rhoGCA, Edge const& edge) const
{
    uint32 nbrNodes    = 0;
    uint32 iStartPatch = 0;
    uint32 iStartGCA   = 0;

    getGCAandPatchStartIndexes_(patchLayout, rhoPatch, rhoGCA, edge, Direction::X, nbrNodes,
                                iStartPatch, iStartGCA);

    // HERE we add !
    for (uint32 iNode = 0; iNode < nbrNodes; ++iNode)
    {
        rhoPatch(iStartPatch + iNode) += rhoGCA(iStartGCA + iNode);
    }
}


void PatchBoundary::addGCAFluxesToPatch1D_(GridLayout const& patchLayout, Ions& ionsPatch,
                                           Ions const& ionsGCA, Edge const& edge) const
{
    uint32 iCompX = static_cast<uint32>(Direction::X);
    uint32 iCompY = static_cast<uint32>(Direction::Y);
    uint32 iCompZ = static_cast<uint32>(Direction::Z);

    for (uint32 ispe = 0; ispe < ionsPatch.nbrSpecies(); ++ispe)
    {
        uint32 nbrNodes    = 0;
        uint32 iStartPatch = 0;
        uint32 iStartGCA   = 0;

        Species& speciesPatch     = ionsPatch.species(ispe);
        Species const& speciesGCA = ionsGCA.species(ispe);

        std::array<std::reference_wrapper<Field>, 3> fxyzPatch
            = {{speciesPatch.flux(iCompX), speciesPatch.flux(iCompY), speciesPatch.flux(iCompZ)}};

        std::array<std::reference_wrapper<Field const>, 3> fxyzGCA
            = {{speciesGCA.flux(iCompX), speciesGCA.flux(iCompY), speciesGCA.flux(iCompZ)}};

        for (uint32 ifield = 0; ifield < fxyzPatch.size(); ++ifield)
        {
            Field& fluxPatch     = fxyzPatch[ifield];
            Field const& fluxGCA = fxyzGCA[ifield];

            getGCAandPatchStartIndexes_(patchLayout, fluxPatch, fluxGCA, edge, Direction::X,
                                        nbrNodes, iStartPatch, iStartGCA);

            for (uint32 iNode = 0; iNode < nbrNodes; ++iNode)
                fluxPatch(iStartPatch + iNode) += fluxGCA(iStartGCA + iNode);
        }
    }
}



void PatchBoundary::getGCAandPatchStartIndexes_(GridLayout const& patchLayout,
                                                Field const& fieldPatch, Field const& fieldGCA,
                                                Edge const& edge, Direction const& direction,
                                                uint32& nbrNodes, uint32& iStartPatch,
                                                uint32& iStartGCA) const
{
    // here we need the exact number of ghost nodes
    // no exception for 1st order
    uint32 nbrGhosts = static_cast<uint32>(std::floor(layout_.order() / 2.));

    nbrNodes = 2 * nbrGhosts + 1;

    // Default initialization Edge::Xmin
    iStartPatch = patchLayout.physicalStartIndex(fieldPatch, direction) - nbrGhosts;
    iStartGCA   = layout_.physicalEndIndex(fieldGCA, direction) - nbrGhosts;

    if (edge == Edge::Xmax)
    {
        iStartPatch = patchLayout.physicalEndIndex(fieldPatch, direction) - nbrGhosts;
        iStartGCA   = layout_.physicalStartIndex(fieldGCA, direction) - nbrGhosts;
    }
}



void PatchBoundary::addGCAChargeDensityToPatch2D_(GridLayout const& patchLayout, Field& rhoPatch,
                                                  Field const& rhoGCA, Edge const& edge) const
{
    (void)patchLayout;
    (void)rhoPatch;
    (void)rhoGCA;
    (void)edge;
    throw std::runtime_error("addGCAChargeDensityToPatch2D_ : Not Implemented");
}


void PatchBoundary::addGCAFluxesToPatch2D_(GridLayout const& patchLayout, Ions& ionsPatch,
                                           Ions const& ionsGCA, Edge const& edge) const
{
    (void)patchLayout;
    (void)ionsPatch;
    (void)ionsGCA;
    (void)edge;
    throw std::runtime_error("addGCAFluxesToPatch2D_ : Not Implemented");
}



void PatchBoundary::addGCAChargeDensityToPatch3D_(GridLayout const& patchLayout, Field& rhoPatch,
                                                  Field const& rhoGCA, Edge const& edge) const
{
    (void)patchLayout;
    (void)rhoPatch;
    (void)rhoGCA;
    (void)edge;
    throw std::runtime_error("addGCAChargeDensityToPatch3D_ : Not Implemented");
}


void PatchBoundary::addGCAFluxesToPatch3D_(GridLayout const& patchLayout, Ions& ionsPatch,
                                           Ions const& ionsGCA, Edge const& edge) const
{
    (void)patchLayout;
    (void)ionsPatch;
    (void)ionsGCA;
    (void)edge;
    throw std::runtime_error("addGCAFluxesToPatch3D_ : Not Implemented");
}



void PatchBoundary::updateCorrectedEMfields(GridLayout const& parentLayout,
                                            Electromag const& parentElectromag)
{
    // We update the electromagnetic field on the GCA layout
    // GCA == PatchBoundary, we just use the private layout_
    ElectromagInitializer emInitializer{layout_, "_EMField", "_EMFields"};

    // A linear interpolator is enough here (= 1)
    Interpolator interpolator(1);

    // Now we compute the E and B fields
    // of the ElectromagInitializer
    fieldAtRefinedNodes(interpolator, parentLayout, parentElectromag, layout_, emInitializer);

    // We now update the correctedEM field of the GCA
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
