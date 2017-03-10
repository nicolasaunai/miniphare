
#include "BoundaryConditions/boundary_conditions.h"
#include "BoundaryConditions/patchboundarycondition.h"
#include "BoundaryConditions/patchboundary.h"

#include "AMR/mlmdinitializerfactory.h"
#include "AMR/mlmdparticleinitializer.h"
#include "AMR/particleselector.h"

#include "Electromag/electromag.h"
#include "Interpolator/interpolator.h"


#include <cmath>





std::unique_ptr<IonsInitializer> MLMDInitializerFactory::createIonsInitializer() const
{
    /* this routine creates an ion initializer with a Patch Choice function. */
    std::unique_ptr<IonsInitializer> ionInitPtr{ new IonsInitializer{} };
    Ions const& parentIons = parentPatch_->ions();

    for (uint32 ispe=0; ispe < parentIons.nbrSpecies(); ++ispe)
    {
        Species const& species = parentIons.species(ispe);
        Box  parentCoordinates  = parentPatch_->coordinates();
        std::unique_ptr<ParticleSelector> selector{
            new isInBox{parentCoordinates, newPatchCoords_, refinedLayout_.dxdydz()} };

        std::unique_ptr<ParticleInitializer>
                particleInit{new MLMDParticleInitializer{species, std::move(selector) }};

        ionInitPtr->masses.push_back( parentIons.species(ispe).mass() );
        ionInitPtr->particleInitializers.push_back( std::move(particleInit) );
    }


    return ionInitPtr;
}




std::unique_ptr<ElectromagInitializer>
MLMDInitializerFactory::createElectromagInitializer() const
{

    Electromag const & parentElectromag = parentPatch_->data().EMfields() ;

    Interpolator interpolator(interpolationOrder_) ;

    std::unique_ptr<ElectromagInitializer> eminit {
        new ElectromagInitializer{refinedLayout_, "_EMField", "_EMFields"} };

    std::cout << "creating MLMD ElectromagInitializer" << std::endl;

    fieldAtRefinedNodes1D( interpolator,
                           parentPatch_->layout(),
                           parentElectromag.getE() , parentElectromag.getB(),
                           refinedLayout_,
                           eminit->E_ , eminit->B_ ) ;

    return eminit;
}




std::unique_ptr<SolverInitializer> MLMDInitializerFactory::createSolverInitializer() const
{
    std::unique_ptr<SolverInitializer> solverInitPtr{ new SolverInitializer{} };


    solverInitPtr->pusherType =
           parentPatch_->solver().getPusherType() ;

    solverInitPtr->interpolationOrders =
            parentPatch_->solver().getInterpolationOrders() ;

    return  solverInitPtr;
}


std::unique_ptr<OhmInitializer> MLMDInitializerFactory::createOhmInitializer() const
{
    return nullptr;
}


std::unique_ptr<BoundaryCondition> MLMDInitializerFactory::createBoundaryCondition() const
{

    const std::array<uint32, 3> nbrBCTab{ {2, 4, 8} } ;

    GridLayout coarseLayout{parentPatch_->layout()} ;

    uint32 nbrBoundaries = nbrBCTab[coarseLayout.nbDimensions()-1] ;

    PRA refinedPRA{ buildPRA_(refinedLayout_) } ;

    std::vector<std::unique_ptr<Boundary>> boundaries{} ;

    // FIRST, LOOP OVER all the boundaries
    for(uint32 ibord=0 ; ibord<nbrBoundaries ; ++ibord)
    {
        // Get a sub layout of the patch layout
        // corresponding to the adequate PRA boundary
        GridLayout praEdgeLayout{ buildPRABoundaryLayout_( refinedPRA, ibord ) };

        /* this routine creates an ion initializer with a Patch Choice function. */
        std::unique_ptr<IonsInitializer> ionInitPtr{ new IonsInitializer{} };
        Ions const& parentIons = parentPatch_->ions();

        for (uint32 ispe=0; ispe < parentIons.nbrSpecies(); ++ispe)
        {
            Species const& species = parentIons.species(ispe);
            // unused: Box  parentCoordinates  = parentPatch_->coordinates();

            // TODO: define isInPRA particle selector
            std::unique_ptr<ParticleSelector> selector{
                new isInPRA{} };

            std::unique_ptr<ParticleInitializer>
                    particleInit{new MLMDParticleInitializer{species, std::move(selector) }};

            ionInitPtr->masses.push_back( parentIons.species(ispe).mass() );
            ionInitPtr->particleInitializers.push_back( std::move(particleInit) );
        }


        // WARNING: TODO: we need a sublayout of refinedLayout_
        // corresponding to the sub Box
        std::unique_ptr<ElectromagInitializer> emInitPtr {
            new ElectromagInitializer{praEdgeLayout, "_EMField", "_EMFields"} };

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



GridLayout MLMDInitializerFactory::buildPRABoundaryLayout_(
        PRA const & refinedPRA, uint32 ibord ) const
{

    std::array<double,3> dxdydz = refinedLayout_.dxdydz();

    uint32 nbDims = refinedLayout_.nbDimensions() ;
    std::string layoutName = refinedLayout_.layoutName() ;

    uint32 ghostParameter = refinedLayout_.order() ;


    LogicalBox logic = refinedPRA.logicDecomposition[ibord] ;
    Box box = refinedPRA.boxDecomposition[ibord] ;

    // TODO: find out the adequate origin
    Point origin{box.x0, box.y0, box.z0} ;

    // TODO: find out the required number of cells in each direction
    uint32 nbrCellx = logic.ix1 - logic.ix0 -1 ;
    uint32 nbrCelly = logic.iy1 - logic.iy0 -1 ;
    uint32 nbrCellz = logic.iz1 - logic.iz0 -1 ;

    switch( ibord )
    {
    case 0:
        nbrCelly = refinedLayout_.nbrCelly() + refinedPRA.nbrCells[1] ;
        nbrCellz = refinedLayout_.nbrCellz() + refinedPRA.nbrCells[2] ;
        break;
    case 1:
        nbrCelly = refinedLayout_.nbrCelly() + refinedPRA.nbrCells[1] ;
        nbrCellz = refinedLayout_.nbrCellz() + refinedPRA.nbrCells[2] ;
        break;
    case 2:
        nbrCellz = refinedLayout_.nbrCellz() + refinedPRA.nbrCells[2] ;
        break;
    case 3:
        nbrCellz = refinedLayout_.nbrCellz() + refinedPRA.nbrCells[2] ;
        break;
    case 4:
        break;
    case 5:
        break;
    default:
        throw std::runtime_error("Wrong boundary identifier");
    }

    std::array<uint32,3> nbrCells{ {nbrCellx, nbrCelly, nbrCellz} } ;


    return  GridLayout(dxdydz, nbrCells,
                       nbDims, layoutName,
                       origin, ghostParameter );
}




PRA MLMDInitializerFactory::buildPRA_( GridLayout const & layout ) const
{
    PRA newPRA{} ;

    switch (layout.nbDimensions())
    {
    case 1:
        newPRA = buildPRA1D_( layout );
        break;
    case 2:
        newPRA = buildPRA2D_( layout );
        break;
    case 3:
        newPRA = buildPRA3D_( layout );
        break;
    default:
        throw std::runtime_error("wrong dimensionality");
    }

    return newPRA ;
}


PRA MLMDInitializerFactory::buildPRA1D_( GridLayout const & layout ) const
{
    uint32 nbrMaxGhost = std::max( layout.nbrGhostCells(QtyCentering::primal),
                                   layout.nbrGhostCells(QtyCentering::dual) ) ;

    std::array<uint32, 3> nbrCells{ { 2*nbrMaxGhost, 0, 0 } } ;

    uint32 ix0_in, ix1_in, ix0_out, ix1_out ;
    double x0_in, x1_in, x0_out, x1_out ;

    preCompute_( layout, Direction::X, nbrMaxGhost,
                   ix0_in , ix1_in ,
                   ix0_out, ix1_out,
                   x0_in  , x1_in  ,
                   x0_out , x1_out ) ;

    // Build logic (primal) decomposition
    std::vector<LogicalBox> logicBoxes = { {ix0_out, ix0_in}, {ix1_in, ix1_out} } ;

    // Build decomposition into (primal) boxes
    std::vector<Box> boxes = { {x0_out, x0_in}, {x1_in, x1_out} } ;

    return PRA{nbrCells, logicBoxes, boxes} ;
}




void MLMDInitializerFactory::preCompute_( GridLayout const & layout,
                                            Direction direction,
                                            uint32 nbrMaxGhost,
                                            uint32 & ix0_in , uint32 & ix1_in ,
                                            uint32 & ix0_out, uint32 & ix1_out,
                                            double & x0_in  , double & x1_in ,
                                            double & x0_out , double & x1_out ) const
{
    // Inner primal indexes
    ix0_in = layout.physicalStartIndex(QtyCentering::primal, direction) + nbrMaxGhost ;
    ix1_in = layout.physicalEndIndex  (QtyCentering::primal, direction) - nbrMaxGhost ;

    // Outer primal indexes
    ix0_out = layout.physicalStartIndex(QtyCentering::primal, direction) - nbrMaxGhost ;
    ix1_out = layout.physicalEndIndex  (QtyCentering::primal, direction) + nbrMaxGhost ;

    // Inner box coordinates (primal)
    x0_in = ix0_in*layout.dx() + layout.origin().x_ ;
    x1_in = ix1_in*layout.dx() + layout.origin().x_ ;

    // Outer box coordinates (primal)
    x0_out = ix0_out*layout.dx() + layout.origin().x_ ;
    x1_out = ix1_out*layout.dx() + layout.origin().x_ ;

}




PRA MLMDInitializerFactory::buildPRA2D_( GridLayout const & layout ) const
{
    uint32 nbrMaxGhost = std::max( layout.nbrGhostCells(QtyCentering::primal),
                                   layout.nbrGhostCells(QtyCentering::dual) ) ;

    std::array<uint32, 3> nbrCells{ { 2*nbrMaxGhost, 2*nbrMaxGhost, 0 } } ;

    uint32 ix0_in, ix1_in, ix0_out, ix1_out ;
    uint32 iy0_in, iy1_in, iy0_out, iy1_out ;
    double x0_in, x1_in, x0_out, x1_out ;
    double y0_in, y1_in, y0_out, y1_out ;

    preCompute_( layout, Direction::X, nbrMaxGhost,
                   ix0_in , ix1_in ,
                   ix0_out, ix1_out,
                   x0_in  , x1_in  ,
                   x0_out , x1_out ) ;

    preCompute_( layout, Direction::Y, nbrMaxGhost,
                   iy0_in , iy1_in ,
                   iy0_out, iy1_out,
                    y0_in  , y1_in  ,
                    y0_out , y1_out ) ;

    // Build logic (primal) decomposition
    std::vector<LogicalBox> logicBoxes = { {ix0_out, ix0_in , iy0_out, iy1_out},
                                           {ix1_in , ix1_out, iy0_out, iy1_out},
                                           {ix0_in , ix1_in , iy0_out, iy0_in },
                                           {ix0_in , ix1_in , iy1_in , iy1_out} } ;

    // Build decomposition into (primal) boxes
    std::vector<Box> boxes = { {x0_out, x0_in , y0_out, y1_out},
                               {x1_in , x1_out, y0_out, y1_out},
                               {x0_in , x1_in , y0_out, y0_in },
                               {x0_in , x1_in , y1_in , y1_out} } ;

    return PRA{nbrCells, logicBoxes, boxes} ;
}


PRA MLMDInitializerFactory::buildPRA3D_( GridLayout const & layout ) const
{
    uint32 nbrMaxGhost = std::max( layout.nbrGhostCells(QtyCentering::primal),
                                   layout.nbrGhostCells(QtyCentering::dual) ) ;

    std::array<uint32, 3> nbrCells{ { 2*nbrMaxGhost, 2*nbrMaxGhost, 2*nbrMaxGhost } } ;

    uint32 ix0_in, ix1_in, ix0_out, ix1_out ;
    uint32 iy0_in, iy1_in, iy0_out, iy1_out ;
    uint32 iz0_in, iz1_in, iz0_out, iz1_out ;
    double x0_in, x1_in, x0_out, x1_out ;
    double y0_in, y1_in, y0_out, y1_out ;
    double z0_in, z1_in, z0_out, z1_out ;

    preCompute_( layout, Direction::X, nbrMaxGhost,
                   ix0_in , ix1_in ,
                   ix0_out, ix1_out,
                   x0_in  , x1_in  ,
                   x0_out , x1_out ) ;

    preCompute_( layout, Direction::Y, nbrMaxGhost,
                   iy0_in , iy1_in ,
                   iy0_out, iy1_out,
                    y0_in  , y1_in  ,
                    y0_out , y1_out ) ;

    preCompute_( layout, Direction::Z, nbrMaxGhost,
                   iz0_in , iz1_in ,
                   iz0_out, iz1_out,
                    z0_in  , z1_in  ,
                    z0_out , z1_out ) ;

    // Build logic (primal) decomposition
    // TODO: 3D generalization
    std::vector<LogicalBox> logicBoxes = { {ix0_out, ix0_in}, {ix1_in, ix1_out} } ;

    // Build decomposition into (primal) boxes
    // TODO: 3D generalization
    std::vector<Box> boxes = { {x0_out, x0_in}, {x1_in, x1_out} } ;

    return PRA{nbrCells, logicBoxes, boxes} ;
}



GridLayout const& MLMDInitializerFactory::gridLayout() const
{
    return refinedLayout_;
}


Box MLMDInitializerFactory::getRefinedBox() const
{
    return refinedLayout_.getBox() ;
}


double MLMDInitializerFactory::timeStep() const
{
    return dt_;
}




/**
 * @brief The fieldAtRefinedNodes1D method is used to interpolate the fields
 * from a coarse patch into a refined patch, in the region where the two patches
 * overlap.
 *
 * This algorithm is quite similar to fieldAtParticle1D(...) method,
 * the particleField variable used in the latter method is replaced
 * by the variable refinedField in the present method.
 * But we perform the same kind of operations, we want to know the
 * interpolated value of a field at a specific point (.i.e. particle position)
 * using the coarse grid information.
 *
 *
 * @param interp
 * @param parentLayout
 * @param Eparent
 * @param Bparent
 * @param newLayout
 * @param newE
 * @param newB
 */
void fieldAtRefinedNodes1D(Interpolator const& interp,
                           GridLayout const & coarseLayout,
                           VecField const & Ecoarse , VecField const & Bcoarse,
                           GridLayout const & refinedLayout,
                           VecField & Erefined , VecField & Brefined)
{
    uint32 idirX = static_cast<uint32>(Direction::X) ;
    uint32 idirY = static_cast<uint32>(Direction::Y) ;
    uint32 idirZ = static_cast<uint32>(Direction::Z) ;

    Field const & Ex = Ecoarse.component(idirX) ;
    Field const & Ey = Ecoarse.component(idirY) ;
    Field const & Ez = Ecoarse.component(idirZ) ;

    Field const & Bx = Bcoarse.component(idirX) ;
    Field const & By = Bcoarse.component(idirY) ;
    Field const & Bz = Bcoarse.component(idirZ) ;

    std::vector<std::reference_wrapper<Field const>>
            ExyzBxyzCoarse = {Ex, Ey, Ez, Bx, By, Bz} ;


    Field & ExNew = Erefined.component(idirX) ;
    Field & EyNew = Erefined.component(idirY) ;
    Field & EzNew = Erefined.component(idirZ) ;

    Field & BxNew = Brefined.component(idirX) ;
    Field & ByNew = Brefined.component(idirY) ;
    Field & BzNew = Brefined.component(idirZ) ;

    std::vector<std::reference_wrapper<Field>>
            ExyzBxyzRefined = {ExNew, EyNew, EzNew, BxNew, ByNew, BzNew} ;


    double newOriginReducedOnCoarse =
            std::fabs(refinedLayout.origin().x_ - coarseLayout.origin().x_)
            / coarseLayout.dx() ;

    // loop on Electric field components
    for( uint32 ifield=0 ; ifield < ExyzBxyzCoarse.size() ; ++ifield )
    {
        Field const & coarseField  = ExyzBxyzCoarse[ifield] ;
        Field       & refinedField = ExyzBxyzRefined[ifield] ;

        uint32 iStart = refinedLayout.physicalStartIndex(refinedField, Direction::X);
        uint32 iEnd   = refinedLayout.physicalEndIndex  (refinedField, Direction::X);

        // The parent field centering DOES matter
        auto centering = coarseLayout.fieldCentering( coarseField, Direction::X ) ;

        // Initialize new field
        for( uint32 ix=iStart ; ix<=iEnd ; ++ix )
            refinedField(ix) = 0. ;

        // loop on new field indexes
        for( uint32 ix=iStart ; ix<=iEnd ; ++ix )
        {
            // ix coordinate can be seen as the Particle position of
            // the method fieldAtParticle1D(...)
            //
            // Compute the reduced coordinate
            // on the parent GridLayout
            double delta = ix * refinedLayout.dx() / coarseLayout.dx() ;

            double coord = newOriginReducedOnCoarse + delta ;

            // WARNING: coord is a reduced coordinate
            // on the parent GridLayout
            auto indexesAndWeights = interp.getIndexesAndWeights( coord, centering ) ;

            std::vector<uint32> indexes = std::get<0>(indexesAndWeights) ;
            std::vector<double> weights = std::get<1>(indexesAndWeights) ;

            // nodes (stored in indexes) from the parent layout now contribute
            // to the node: ix, located on the new layout
            for(uint32 ik=0 ; ik<indexes.size() ; ++ik)
            {
                refinedField(ix) += coarseField(indexes[ik]) * weights[ik] ;
            }
        }
    }


}






