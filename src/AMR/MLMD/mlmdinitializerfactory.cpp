
#include <algorithm>
#include <cmath>


#include "AMR/MLMD/pra.h"
#include "AMR/coarsetorefinemesh.h"
#include "AMR/particleselector.h"

#include "AMR/MLMD/mlmdinitializerfactory.h"
#include "AMR/MLMD/mlmdparticleinitializer.h"


#include "Electromag/electromag.h"

#include "Interpolator/interpolator.h"

#include "Splitting/splittingstrategyfactory.h"

#include "BoundaryConditions/boundary_conditions.h"
#include "BoundaryConditions/patchboundary.h"
#include "BoundaryConditions/patchboundarycondition.h"



std::array<double, 3> computeNearPRARegion(uint32 interpOrder, std::array<double, 3> const& dxdydz);


/**
 * @brief MLMDInitializerFactory::createIonsInitializer creates an
 * IonsInitializer, it will contain a ParticleInitializer for
 * each ion species we need to initialize.
 *
 * Operations related to the construction of ParticleInitializer
 * objects are handled by
 * MLMDInitializerFactory::buildIonsInitializer_(...)
 *
 *
 * @return
 */
std::unique_ptr<IonsInitializer> MLMDInitializerFactory::createIonsInitializer() const
{
    std::cout << "\nCREATE IONS INITIALIZER" << std::endl;

    /* this routine creates an ion initializer with a Patch Choice function. */
    std::unique_ptr<IonsInitializer> ionInitPtr{new IonsInitializer{}};

    std::array<double, 3> tol_xyz{
        computeNearPRARegion(parentPatch_->layout().order(), parentPatch_->layout().dxdydz())};

    Box selectionBox{newPatchCoords_};
    selectionBox.expand(tol_xyz);

    // the ParticleSelector will be shared by
    // multiple species
    std::shared_ptr<ParticleSelector> motherParticleSelector{
        new IsInBoxSelector{parentPatch_->layout(), selectionBox}};


    buildIonsInitializer_(*ionInitPtr, motherParticleSelector, refinedLayout_);


    return ionInitPtr;
}


/**
 * @brief MLMDInitializerFactory::buildIonsInitializer_
 *
 * We build the private attributes of MLMDParticleInitializer,
 * among those attributes the following depend on
 * the species:
 * - the particle source (known from the parent)
 * - the splitting strategy, because each species has its
 * own interpolation order
 *
 * @param ionInit
 * @param selector
 */
void MLMDInitializerFactory::buildIonsInitializer_(IonsInitializer& ionInit,
                                                   std::shared_ptr<ParticleSelector> selector,
                                                   GridLayout const& targetLayout) const
{
    Ions const& parentIons = parentPatch_->ions();

    ionInit.nbrSpecies = parentIons.nbrSpecies();

    for (uint32 ispe = 0; ispe < parentIons.nbrSpecies(); ++ispe)
    {
        SplittingStrategyFactory factory{splitMethods_[ispe], interpolationOrders_[ispe],
                                         refinementRatio_};

        std::unique_ptr<SplittingStrategy> splitting = factory.createSplittingStrategy();

        Species const& species = parentIons.species(ispe);

        std::unique_ptr<ParticleInitializer> particleInit{
            new MLMDParticleInitializer{species, selector, std::move(splitting),
                                        parentPatch_->layout(), targetLayout, refinementRatio_}};

        ionInit.masses.push_back(parentIons.species(ispe).mass());
        ionInit.names.push_back(parentIons.species(ispe).name());
        ionInit.particleInitializers.push_back(std::move(particleInit));
    }
}



/**
 * @brief MLMDInitializerFactory::createElectromagInitializer creates an
 * ElectromagInitializer object containing the adequate data built
 * from the overlying parent patch.
 * This data will be used to initialize the electromagnetic field of
 * a refined patch.
 */
std::unique_ptr<ElectromagInitializer> MLMDInitializerFactory::createElectromagInitializer() const
{
    Electromag const& parentElectromag = parentPatch_->data().EMfields();
    GridLayout const& coarseLayout     = parentPatch_->layout();

    Interpolator interpolator(
        *std::max_element(interpolationOrders_.begin(), interpolationOrders_.end()));

    std::unique_ptr<ElectromagInitializer> eminit{
        new ElectromagInitializer{refinedLayout_, "_EMField", "_EMFields"}};

    std::cout << "creating MLMD ElectromagInitializer" << std::endl;

    fieldAtRefinedNodes(interpolator, coarseLayout, parentElectromag, refinedLayout_, *eminit);

    return eminit;
}




std::unique_ptr<SolverInitializer> MLMDInitializerFactory::createSolverInitializer() const
{
    std::unique_ptr<SolverInitializer> solverInitPtr{new SolverInitializer{}};
    solverInitPtr->pusherType          = pusher_;
    solverInitPtr->interpolationOrders = interpolationOrders_;
    return solverInitPtr;
}




/**
 * @brief MLMDInitializerFactory::createBoundaryCondition is responsible to create
 * a BoundaryCondition object.
 * We build the following private attributes of BoundaryCondition:
 * - PRA refinedPRA_
 * - std::shared_ptr<Patch> parent_
 * - GridLayout coarseLayout_
 * - std::vector<std::unique_ptr<Boundary>> boundaries_
 *
 * In this InitializerFactory, the last attribute has the following concrete type:
 * - std::vector<std::unique_ptr<PatchBoundary>> boundaries
 * we build a PatchBoundary object for each boundary, thereby we
 * must provide an IonInitializer and an ElectromagInitializer containing
 * the adequate data built from the overlying parent patch.
 *
 *
 * @return a unique_ptr to a BoundaryCondition object
 */
std::unique_ptr<BoundaryCondition> MLMDInitializerFactory::createBoundaryCondition() const
{
    std::cout << "\nCREATE BOUNDARY CONDITION" << std::endl;

    GridLayout coarseLayout{parentPatch_->layout()};
    uint32 nbrBoundaries = 2 * coarseLayout.nbDimensions();

    // this will be used to initialize electromagnetic fields
    // at patch boundaries, into PRA layouts
    Electromag const& parentElectromag = parentPatch_->data().EMfields();
    Interpolator interpolator(
        *std::max_element(interpolationOrders_.begin(), interpolationOrders_.end()));
    PRA refinedPRA{buildPRA(refinedLayout_)};

    // We know we are dealing with PatchBoundary objects
    // because we are in a MLMDInitializerFactory method
    std::vector<std::unique_ptr<PatchBoundary>> boundaries{};


    const std::array<Edge, 6> boundaryEdges
        = {{Edge::Xmin, Edge::Xmax, Edge::Ymin, Edge::Ymax, Edge::Zmin, Edge::Zmax}};


    // FIRST, LOOP OVER all the boundaries
    for (uint32 ibord = 0; ibord < nbrBoundaries; ++ibord)
    {
        std::cout << "Boundary, ibord = " << ibord << std::endl;

        // Get the layout of the adequate PRA boundary
        GridLayout praEdgeLayout{buildPRABoundaryLayout(refinedPRA, ibord, refinedLayout_)};
        // Get PRA extended layout
        GridLayout praExtendedLayout{getExtendedLayout_(praEdgeLayout)};

        std::unique_ptr<IonsInitializer> ionInitPtr{new IonsInitializer{}};

        std::array<double, 3> tol_xyz{
            computeNearPRARegion(parentPatch_->layout().order(), parentPatch_->layout().dxdydz())};

        Box selectionBox = praEdgeLayout.getBox();
        selectionBox.expand(tol_xyz);

        // the selector will check whether particles from the parent Box
        // belong to the boundary layout box
        std::shared_ptr<ParticleSelector> selector
            = std::make_shared<IsInBoxSelector>(parentPatch_->layout(), selectionBox);

        buildIonsInitializer_(*ionInitPtr, selector, praEdgeLayout);

        // We need the electromagnetic field on the PRA layout
        // of the adequate Patch boundary
        std::unique_ptr<ElectromagInitializer> emInitPtr{
            new ElectromagInitializer{praExtendedLayout, "_EMField", "_EMFields"}};

        // Now we compute the E and B fields
        // of the ElectromagInitializer
        fieldAtRefinedNodes(interpolator, parentPatch_->layout(), parentElectromag,
                            praExtendedLayout, *emInitPtr);

        // For each boundary build the PatchBoundary object
        std::unique_ptr<PatchBoundary> boundaryPtr{
            new PatchBoundary{praEdgeLayout, praExtendedLayout, std::move(ionInitPtr),
                              std::move(emInitPtr), boundaryEdges[ibord]}};

        // For each boundary add this PatchBoundary to our temporary
        // vector of std::unique_ptr<Boundary>
        boundaries.push_back(std::move(boundaryPtr));
    }

    // SECOND, build PatchBoundaryCondition object
    std::unique_ptr<BoundaryCondition> boundaryCondition{new PatchBoundaryCondition{
        refinedPRA, parentPatch_, refinedLayout_, std::move(boundaries)}};

    return boundaryCondition;
}




std::array<double, 3> computeNearPRARegion(uint32 interpOrder, std::array<double, 3> const& dxdydz)
{
    // In 1D, if the mother particle position is farther than tol_x
    // from the physical boundary of the domain
    // then no child particle will enter the physical domain
    //
    // Exact splitting gives:
    // tol_x = 0.25 * (interpOrder_ + 1) * dx_
    // For generality we overestimate this treshold:
    // tol_x = (interpOrder_ + 1) * dx_
    double tol_x = (interpOrder + 1) * dxdydz[0];
    double tol_y = (interpOrder + 1) * dxdydz[1];
    double tol_z = (interpOrder + 1) * dxdydz[2];

    return {{tol_x, tol_y, tol_z}};
}



/**
 * @brief MLMDInitializerFactory::getExtendedLayout_
 * this method widen slightly the layout provided in argument
 * in each active direction
 *
 *
 * @param praLayout
 * @return
 */
GridLayout MLMDInitializerFactory::getExtendedLayout_(GridLayout const& praLayout) const
{
    Point origin{praLayout.origin()};
    std::array<double, 3> dxdydz{praLayout.dxdydz()};
    std::array<uint32, 3> nxnynz{praLayout.nbrCellxyz()};

    double dx = dxdydz[0];
    double dy = dxdydz[1];
    double dz = dxdydz[2];

    uint32 nx = nxnynz[0];
    uint32 ny = nxnynz[1];
    uint32 nz = nxnynz[2];

    switch (praLayout.nbDimensions())
    {
        case 1:
            origin.x_ += -dx;
            nx += 2;
            break;

        case 2:
            origin.x_ += -dx;
            origin.y_ += -dy;
            nx += 2;
            ny += 2;
            break;

        case 3:
            origin.x_ += -dx;
            origin.y_ += -dy;
            origin.z_ += -dz;
            nx += 2;
            ny += 2;
            nz += 2;
            break;
    }


    return GridLayout(praLayout.dxdydz(), {{nx, ny, nz}}, praLayout.nbDimensions(),
                      praLayout.layoutName(), origin, praLayout.order());
}
