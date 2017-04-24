
#include <cmath>
#include <algorithm>


#include "AMR/MLMD/pra.h"
#include "AMR/particleselector.h"
#include "AMR/coarsetorefinemesh.h"

#include "AMR/MLMD/mlmdinitializerfactory.h"
#include "AMR/MLMD/mlmdparticleinitializer.h"

#include "Electromag/electromag.h"

#include "Interpolator/interpolator.h"

#include "Splitting/splittingstrategyfactory.h"

#include "BoundaryConditions/patchboundary.h"
#include "BoundaryConditions/boundary_conditions.h"
#include "BoundaryConditions/patchboundarycondition.h"





MLMDInitializerFactory::MLMDInitializerFactory(std::shared_ptr<Patch> parentPatch,
                                               Box const & newPatchCoords,
                                               GridLayout const & refinedLayout,
                                               PatchInfo const& patchInfo)
    : newPatchCoords_{newPatchCoords},
      refinedLayout_{ refinedLayout },
      parentPatch_{parentPatch},
      refinementRatio_{patchInfo.refinementRatio},
      interpolationOrders_{patchInfo.interpOrders},
      pusher_{patchInfo.pusher},
      splitMethods_{patchInfo.splitStrategies}
{

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
void MLMDInitializerFactory::buildIonsInitializer_(IonsInitializer & ionInit,
                                                   std::unique_ptr<ParticleSelector> selector) const
{

    Ions const& parentIons = parentPatch_->ions();

    for (uint32 ispe=0; ispe < parentIons.nbrSpecies(); ++ispe)
    {
        SplittingStrategyFactory factory{splitMethods_[ispe],
                                         interpolationOrders_[ispe],
                                         refinementRatio_} ;

        std::unique_ptr<SplittingStrategy>
                splitting = factory.createSplittingStrategy() ;

        Species const& species = parentIons.species(ispe);

        selector->interpOrder = interpolationOrders_[ispe] ;

        std::unique_ptr<ParticleInitializer>
                particleInit{new MLMDParticleInitializer{
                   species, std::move(selector), std::move(splitting),
                   parentPatch_->layout(), refinedLayout_, refinementRatio_ }};

        ionInit.masses.push_back( parentIons.species(ispe).mass() );
        ionInit.particleInitializers.push_back( std::move(particleInit) );
    }
}




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
    /* this routine creates an ion initializer with a Patch Choice function. */
    std::unique_ptr<IonsInitializer> ionInitPtr{ new IonsInitializer{} };

    std::unique_ptr<ParticleSelector> selectorPtr{
        new isInBox{parentPatch_->coordinates(),
                    newPatchCoords_,
                    parentPatch_->layout().dxdydz()} };

    buildIonsInitializer_( *ionInitPtr, std::move(selectorPtr) ) ;

    return ionInitPtr;
}





/**
 * @brief MLMDInitializerFactory::createElectromagInitializer creates an
 * ElectromagInitializer object containing the adequate data built
 * from the overlying parent patch.
 * This data will be used to initialize the electromagnetic field of
 * a refined patch.
 */
std::unique_ptr<ElectromagInitializer>
MLMDInitializerFactory::createElectromagInitializer() const
{

    Electromag const & parentElectromag = parentPatch_->data().EMfields() ;
    GridLayout const & coarseLayout = parentPatch_->layout() ;

    Interpolator interpolator( *std::max_element(interpolationOrders_.begin(),
                                                 interpolationOrders_.end()   ) ) ;

    std::unique_ptr<ElectromagInitializer> eminit {
        new ElectromagInitializer{refinedLayout_, "_EMField", "_EMFields"} };

    std::cout << "creating MLMD ElectromagInitializer" << std::endl;

    fieldAtRefinedNodes( interpolator,
                         coarseLayout, parentElectromag ,
                         refinedLayout_, *eminit ) ;

    return eminit;
}






std::unique_ptr<SolverInitializer> MLMDInitializerFactory::createSolverInitializer() const
{
    std::unique_ptr<SolverInitializer> solverInitPtr{ new SolverInitializer{} };
    solverInitPtr->pusherType = pusher_ ;
    solverInitPtr->interpolationOrders = interpolationOrders_ ;
    return  solverInitPtr;
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

    GridLayout coarseLayout{parentPatch_->layout()};
    uint32 nbrBoundaries = 2 * coarseLayout.nbDimensions();

    // this will be used to initialize electromagnetic fields
    // at patch boundaries, into PRA layouts
    Electromag const & parentElectromag = parentPatch_->data().EMfields();
    Interpolator interpolator( *std::max_element(interpolationOrders_.begin(),
                                                 interpolationOrders_.end()));
    PRA refinedPRA{ buildPRA(refinedLayout_)} ;

    // We know we are dealing with PatchBoundary objects
    // because we are in a MLMDInitializerFactory method
    std::vector<std::unique_ptr<PatchBoundary>> boundaries{} ;




    // FIRST, LOOP OVER all the boundaries
    for(uint32 ibord=0 ; ibord<nbrBoundaries ; ++ibord)
    {
        // Get the layout of the adequate PRA boundary
        GridLayout praEdgeLayout{buildPRABoundaryLayout(refinedPRA, ibord, refinedLayout_)};
        std::unique_ptr<IonsInitializer> ionInitPtr{ new IonsInitializer{}};

        // the selector will check whether particles from the parent Box
        // belong to the boundary layout box
        std::unique_ptr<ParticleSelector> selectorPtr{new isInBox{parentPatch_->coordinates(),
                                                                  praEdgeLayout.getBox(),
                                                                   parentPatch_->layout().dxdydz()}};

        buildIonsInitializer_(*ionInitPtr, std::move(selectorPtr));

        // We need the electromagnetic field on the PRA layout
        // of the adequate Patch boundary
        std::unique_ptr<ElectromagInitializer> emInitPtr
        {new ElectromagInitializer{praEdgeLayout, "_EMField", "_EMFields"}};

        // Now we compute the E and B fields
        // of the ElectromagInitializer
        fieldAtRefinedNodes(interpolator,parentPatch_->layout(), parentElectromag,
                            praEdgeLayout, *emInitPtr);

        // For each boundary build the PatchBoundary object
        std::unique_ptr<PatchBoundary>
                boundaryPtr{ new PatchBoundary{praEdgeLayout,
                                               std::move(ionInitPtr), std::move(emInitPtr)}};

        // For each boundary add this PatchBoundary to our temporary
        // vector of std::unique_ptr<Boundary>
        boundaries.push_back(std::move(boundaryPtr));
    }

    // SECOND, build PatchBoundaryCondition object
    std::unique_ptr<BoundaryCondition> boundaryCondition
    {new PatchBoundaryCondition{refinedPRA, parentPatch_, coarseLayout, std::move(boundaries)}};

    return boundaryCondition;
}


