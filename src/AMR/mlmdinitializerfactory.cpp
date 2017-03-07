
#include "BoundaryConditions/boundary_conditions.h"
#include "BoundaryConditions/patchboundarycondition.h"

#include "AMR/mlmdinitializerfactory.h"
#include "AMR/mlmdparticleinitializer.h"
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

    GridLayout refineLayout{parentPatch_->layout()} ;

    PRA refinedPRA{ buildPRA_(refineLayout) } ;

    // First, build PatchBoundaryCondition object
    std::unique_ptr<BoundaryCondition> bc
    {new PatchBoundaryCondition{refinedPRA, parentPatch_, refineLayout}};




    // Second, next build all the PatchBoundary objects

    // the following intializer factory might be used to build
    // each PatchBoundary object
//    std::unique_ptr<InitializerFactory>
//            factory { new MLMDInitializerFactory(coarsePatch, refinedBox, refinedLayout) }






    // return hard coded domain periodic boundary condition
//    std::vector<DomainBoundaryCondition::BoundaryInfo> boundaries(2);

    // "first" is the edge coordinate
//    boundaries[0].first = Edge::Xmin;
//    boundaries[1].first = Edge::Xmax;

    // "second" is the type of boundary, here periodic
//    boundaries[0].second = BoundaryType::Periodic;
//    boundaries[1].second = BoundaryType::Periodic;

//    std::unique_ptr<BoundaryCondition> bc {new DomainBoundaryCondition{layout_, boundaries}};

    return nullptr;
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
    default:
        throw std::runtime_error("wrong dimensionality");
    }

    return newPRA ;
}


PRA MLMDInitializerFactory::buildPRA1D_( GridLayout const & layout ) const
{
    uint32 nbrMaxGhost = std::max( layout.nbrGhostCells(QtyCentering::primal),
                                   layout.nbrGhostCells(QtyCentering::dual) ) ;

    uint32 ix0_in = layout.physicalStartIndex(QtyCentering::primal, Direction::X) + nbrMaxGhost ;
    uint32 ix1_in = layout.physicalEndIndex  (QtyCentering::primal, Direction::X) - nbrMaxGhost ;

    double x0_in = ix0_in*layout.dx() + layout.origin().x_ ;
    double x1_in = ix1_in*layout.dx() + layout.origin().x_ ;
    Box innerBox{ x0_in, x1_in, 0., 0., 0., 0.} ;


    uint32 ix0_out = layout.ghostStartIndex(QtyCentering::primal, Direction::X);
    uint32 ix1_out = layout.ghostEndIndex  (QtyCentering::primal, Direction::X) ;

    double x0_out = ix0_out*layout.dx() + layout.origin().x_ ;
    double x1_out = ix1_out*layout.dx() + layout.origin().x_ ;
    Box outerBox{ x0_out, x1_out, 0., 0., 0., 0.} ;

    return PRA{innerBox, outerBox} ;
}



PRA MLMDInitializerFactory::buildPRA2D_( GridLayout const & layout ) const
{

    Box innerBox{ 0., 0., 0., 0., 0., 0. } ;
    Box outerBox{ 0., 0., 0., 0., 0., 0. } ;

    return PRA{innerBox, outerBox} ;
}


PRA MLMDInitializerFactory::buildPRA3D_( GridLayout const & layout ) const
{

    Box innerBox{ 0., 0., 0., 0., 0., 0. } ;
    Box outerBox{ 0., 0., 0., 0., 0., 0. } ;

    return PRA{innerBox, outerBox} ;
}



GridLayout const& MLMDInitializerFactory::gridLayout() const
{
    return refinedLayout_;
}


Box MLMDInitializerFactory::getBox() const
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






