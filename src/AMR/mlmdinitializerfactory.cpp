
#include "BoundaryConditions/boundary_conditions.h"
#include "BoundaryConditions/patchboundarycondition.h"
#include "BoundaryConditions/patchboundary.h"

#include "AMR/mlmdinitializerfactory.h"
#include "AMR/mlmdparticleinitializer.h"
#include "AMR/particleselector.h"

#include "Electromag/electromag.h"
#include "Interpolator/interpolator.h"
#include "Interpolator/particlemesh.h"


#include <cmath>
#include <algorithm>




void MLMDInitializerFactory::buildIonsInitializer_(
        IonsInitializer & ionInit,
        ParticleSelector const & selector ) const
{

    Ions const& parentIons = parentPatch_->ions();

    for (uint32 ispe=0; ispe < parentIons.nbrSpecies(); ++ispe)
    {
        Species const& species = parentIons.species(ispe);

        std::unique_ptr<ParticleInitializer>
                particleInit{new MLMDParticleInitializer{species, selector }};

        ionInit.masses.push_back( parentIons.species(ispe).mass() );
        ionInit.particleInitializers.push_back( std::move(particleInit) );
    }

}



std::unique_ptr<IonsInitializer> MLMDInitializerFactory::createIonsInitializer() const
{
    /* this routine creates an ion initializer with a Patch Choice function. */
    std::unique_ptr<IonsInitializer> ionInitPtr{ new IonsInitializer{} };

    std::unique_ptr<ParticleSelector> selectorPtr{
        new isInBox{parentPatch_->coordinates(),
                    newPatchCoords_,
                    parentPatch_->layout().dxdydz()} };

    buildIonsInitializer_( *ionInitPtr, *selectorPtr ) ;

    return ionInitPtr;
}



/**
 * @brief MLMDInitializerFactory::createElectromagInitializer creates an
 * ElectromagInitializer object containing the adequate data built
 * from the overlying parent patch.
 * This data will be used to initialize the electromagnetic field of
 * a refined patch.
 *
 *
 * @return
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


std::unique_ptr<OhmInitializer> MLMDInitializerFactory::createOhmInitializer() const
{
    return nullptr;
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

    GridLayout coarseLayout{parentPatch_->layout()} ;

    uint32 nbrBoundaries = 2* coarseLayout.nbDimensions() ;

    // this will be used to initialize electromagnetic fields
    // at patch boundaries, into PRA layouts
    Electromag const & parentElectromag = parentPatch_->data().EMfields() ;

    Interpolator interpolator( *std::max_element(interpolationOrders_.begin(),
                                                 interpolationOrders_.end()   ) ) ;

    PRA refinedPRA{ buildPRA_(refinedLayout_) } ;

    std::vector<std::unique_ptr<Boundary>> boundaries{} ;

    // FIRST, LOOP OVER all the boundaries
    for(uint32 ibord=0 ; ibord<nbrBoundaries ; ++ibord)
    {
        // Get the layout of the adequate PRA boundary
        GridLayout praEdgeLayout{ buildPRABoundaryLayout_( refinedPRA, ibord ) };

        std::unique_ptr<IonsInitializer> ionInitPtr{ new IonsInitializer{} };

        // the selector will check whether particles from the parent Box
        // belong to the boundary layout box
        std::unique_ptr<ParticleSelector> selectorPtr{
            new isInBox{parentPatch_->coordinates(),
                        praEdgeLayout.getBox(),
                        parentPatch_->layout().dxdydz()} } ;

        buildIonsInitializer_( *ionInitPtr, *selectorPtr ) ;

        // We need the electromagnetic field on the PRA layout
        // of the adequate Patch boundary
        std::unique_ptr<ElectromagInitializer> emInitPtr {
            new ElectromagInitializer{praEdgeLayout, "_EMField", "_EMFields"} };

        // Now we compute the E and B fields
        // of the ElectromagInitializer
        fieldAtRefinedNodes( interpolator,
                             parentPatch_->layout(), parentElectromag ,
                             praEdgeLayout, *emInitPtr ) ;

        // For each boundary build the PatchBoundary object
        std::unique_ptr<Boundary>
                boundaryPtr{ new PatchBoundary{praEdgeLayout,
                        std::move(ionInitPtr), std::move(emInitPtr)} };


        // For each boundary add this PatchBoundary to our temporary
        // vector of std::unique_ptr<Boundary>
        boundaries.push_back( std::move(boundaryPtr) ) ;

    }

    // SECOND, build PatchBoundaryCondition object
    std::unique_ptr<BoundaryCondition> boundaryCondition
    {new PatchBoundaryCondition{refinedPRA, parentPatch_, coarseLayout, std::move(boundaries)}};


    return boundaryCondition;
}


/**
 * @brief MLMDInitializerFactory::buildPRABoundaryLayout_ has to
 * return a valid GridLayout corrsponding to a part of a complete PRA.
 * In 1D, we have 2 possible boundaries.
 * In 2D, we have 4 possible boundaries.
 * In 3D, we have 6 possible boundaries.
 *
 * @param refinedPRA
 * @param ibord
 * @return
 */
GridLayout MLMDInitializerFactory::buildPRABoundaryLayout_(
        PRA const & refinedPRA, uint32 ibord ) const
{

    std::array<double,3> dxdydz = refinedLayout_.dxdydz();

    uint32 nbDims = refinedLayout_.nbDimensions() ;
    std::string layoutName = refinedLayout_.layoutName() ;

    uint32 ghostParameter = refinedLayout_.order() ;


    LogicalBox logic = refinedPRA.logicDecomposition[ibord] ;
    Box box = refinedPRA.boxDecomposition[ibord] ;

    Point origin{box.x0, box.y0, box.z0} ;

    uint32 nbrCellx = logic.ix1 - logic.ix0 ;
    uint32 nbrCelly = logic.iy1 - logic.iy0 ;
    uint32 nbrCellz = logic.iz1 - logic.iz0 ;

    std::array<uint32,3> nbrCells{ {nbrCellx, nbrCelly, nbrCellz} } ;


    return  GridLayout(dxdydz, nbrCells,
                       nbDims, layoutName,
                       origin, ghostParameter );
}


/**
 * @brief MLMDInitializerFactory::buildPRA_ has to return a valid
 * PRA depending on the dimension.
 * In 1D, the PRA splits into 2 segments.
 * In 2D, the PRA splits into 4 rectangle areas.
 * In 3D, the PRA splits into 6 cuboid volumes.
 *
 * @param layout
 * @return
 */
PRA MLMDInitializerFactory::buildPRA_( GridLayout const & layout ) const
{
    switch (layout.nbDimensions())
    {
    case 1:
        return buildPRA1D_( layout );

    case 2:
        return buildPRA2D_( layout );

    case 3:
        return buildPRA3D_( layout );

    default:
        throw std::runtime_error("wrong dimensionality");
    }
}


/**
 * @brief MLMDInitializerFactory::buildPRA1D_ returns a simple
 * 1D PRA made of 2 segments
 *
 * @param layout
 * @return
 */
PRA MLMDInitializerFactory::buildPRA1D_( GridLayout const & layout ) const
{
    std::array<uint32, 3> PRAwidth{ { 2*PRAHalfWidth_(layout), 0, 0 } } ;

    uint32 ix0_in, ix1_in, ix0_out, ix1_out ;
    double x0_in, x1_in, x0_out, x1_out ;

    definePRA1Dlimits_( layout, Direction::X,
                        PRAHalfWidth_(layout),
                        ix0_in , ix1_in ,
                        ix0_out, ix1_out,
                        x0_in  , x1_in  ,
                        x0_out , x1_out ) ;

    // Build logic (primal) decomposition
    std::vector<LogicalBox> logicBoxes = { {ix0_out, ix0_in}, {ix1_in, ix1_out} } ;

    // Build decomposition into (primal) boxes
    std::vector<Box> boxes = { {x0_out, x0_in}, {x1_in, x1_out} } ;

    return PRA{PRAwidth, logicBoxes, boxes} ;
}


/**
 * @brief MLMDInitializerFactory::buildPRA2D_ returns a 2D PRA
 * made of 4 rectangle areas
 *
 *
 * @param layout
 * @return
 */
PRA MLMDInitializerFactory::buildPRA2D_( GridLayout const & layout ) const
{

    std::array<uint32, 3> PRAwidth{ { 2*PRAHalfWidth_(layout), 2*PRAHalfWidth_(layout), 0 } } ;

    uint32 ix0_in, ix1_in, ix0_out, ix1_out ;
    uint32 iy0_in, iy1_in, iy0_out, iy1_out ;
    double x0_in, x1_in, x0_out, x1_out ;
    double y0_in, y1_in, y0_out, y1_out ;

    definePRA1Dlimits_( layout, Direction::X,
                        PRAHalfWidth_(layout),
                        ix0_in , ix1_in ,
                        ix0_out, ix1_out,
                        x0_in  , x1_in  ,
                        x0_out , x1_out ) ;

    definePRA1Dlimits_( layout, Direction::Y,
                        PRAHalfWidth_(layout),
                        iy0_in , iy1_in ,
                        iy0_out, iy1_out,
                        y0_in  , y1_in  ,
                        y0_out , y1_out ) ;

    // Build logic (primal) decomposition
    // TODO: provide a link to redmine with a drawing
    // to check rapidly the points
    std::vector<LogicalBox> logicBoxes = { {ix0_out, ix0_in , iy0_out, iy1_out},
                                           {ix1_in , ix1_out, iy0_out, iy1_out},
                                           {ix0_in , ix1_in , iy0_out, iy0_in },
                                           {ix0_in , ix1_in , iy1_in , iy1_out} } ;

    // Build decomposition into (primal) boxes
    std::vector<Box> boxes = { {x0_out, x0_in , y0_out, y1_out},
                               {x1_in , x1_out, y0_out, y1_out},
                               {x0_in , x1_in , y0_out, y0_in },
                               {x0_in , x1_in , y1_in , y1_out} } ;

    return PRA{PRAwidth, logicBoxes, boxes} ;
}


/**
 * @brief MLMDInitializerFactory::buildPRA3D_ returns a 3D PRA
 * made of 6 cuboid volumes
 *
 *
 * @param layout
 * @return
 */
PRA MLMDInitializerFactory::buildPRA3D_( GridLayout const & layout ) const
{

    std::array<uint32, 3> PRAwidth{ { 2*PRAHalfWidth_(layout), 2*PRAHalfWidth_(layout), 2*PRAHalfWidth_(layout) } } ;

    uint32 ix0_in, ix1_in, ix0_out, ix1_out ;
    uint32 iy0_in, iy1_in, iy0_out, iy1_out ;
    uint32 iz0_in, iz1_in, iz0_out, iz1_out ;
    double x0_in, x1_in, x0_out, x1_out ;
    double y0_in, y1_in, y0_out, y1_out ;
    double z0_in, z1_in, z0_out, z1_out ;

    definePRA1Dlimits_( layout, Direction::X,
                        PRAHalfWidth_(layout),
                        ix0_in , ix1_in ,
                        ix0_out, ix1_out,
                        x0_in  , x1_in  ,
                        x0_out , x1_out ) ;

    definePRA1Dlimits_( layout, Direction::Y,
                        PRAHalfWidth_(layout),
                        iy0_in , iy1_in ,
                        iy0_out, iy1_out,
                        y0_in  , y1_in  ,
                        y0_out , y1_out ) ;

    definePRA1Dlimits_( layout, Direction::Z,
                        PRAHalfWidth_(layout),
                        iz0_in , iz1_in ,
                        iz0_out, iz1_out,
                        z0_in  , z1_in  ,
                        z0_out , z1_out ) ;

    // Build logic (primal) decomposition
    // TODO: 3D generalization
    // TODO: provide a link to redmine with a drawing
    // to check rapidly the points
    std::vector<LogicalBox> logicBoxes = { {ix0_out, ix0_in}, {ix1_in, ix1_out} } ;

    // Build decomposition into (primal) boxes
    // TODO: 3D generalization
    std::vector<Box> boxes = { {x0_out, x0_in}, {x1_in, x1_out} } ;

    return PRA{PRAwidth, logicBoxes, boxes} ;
}

/**
 * @brief MLMDInitializerFactory::definePRA1Dlimits_ is in charge of
 * computing indexes - and the associated coordinates - defining a PRA
 * in a prescribed direction.
 * A PRA can be defined knowing an inner and an outer box
 * (see https://hephaistos.lpp.polytechnique.fr/redmine/projects/hyb-par/wiki/FieldBCs#PRA-and-the-non-connexe-topology-issue)
 *
 *
 * @param layout
 * @param direction
 * @param nbrMaxGhost
 */
void MLMDInitializerFactory::definePRA1Dlimits_( GridLayout const & layout,
                                            Direction direction,
                                            uint32 praHalfWidth,
                                            uint32 & ix0_in , uint32 & ix1_in ,
                                            uint32 & ix0_out, uint32 & ix1_out,
                                            double & x0_in  , double & x1_in ,
                                            double & x0_out , double & x1_out ) const
{
    // Inner primal indexes
    ix0_in = layout.physicalStartIndex(QtyCentering::primal, direction) + praHalfWidth ;
    ix1_in = layout.physicalEndIndex  (QtyCentering::primal, direction) - praHalfWidth ;

    // Outer primal indexes
    ix0_out = layout.physicalStartIndex(QtyCentering::primal, direction) - praHalfWidth ;
    ix1_out = layout.physicalEndIndex  (QtyCentering::primal, direction) + praHalfWidth ;

    // Inner box coordinates (primal)
    x0_in = ix0_in*layout.dx() + layout.origin().getCoord( static_cast<uint32>(direction) ) ;
    x1_in = ix1_in*layout.dx() + layout.origin().getCoord( static_cast<uint32>(direction) ) ;

    // Outer box coordinates (primal)
    x0_out = ix0_out*layout.dx() + layout.origin().getCoord( static_cast<uint32>(direction) ) ;
    x1_out = ix1_out*layout.dx() + layout.origin().getCoord( static_cast<uint32>(direction) ) ;

}




uint32 MLMDInitializerFactory::PRAHalfWidth_( GridLayout const & layout ) const
{
    return layout.nbrGhostCells(QtyCentering::primal) ;
}



