
#include "AMR/mlmdinitializerfactory.h"
#include "AMR/mlmdparticleinitializer.h"
#include "Electromag/electromag.h"
#include "Interpolator/interpolator.h"


#include <cmath>




/* below are just stupid functions to make this initializer works */

std::array<double,3>  zeroMagneticField(double x, double y, double z)
{
    std::array<double,3> vec;
    (void) x;
    (void) y;
    (void) z;
    vec[0] = 1.;
    vec[1] = 0.;
    vec[2] = 0.;
    return vec;
}


std::array<double,3> zeroElectricField(double x, double y, double z)
{
    std::array<double,3> vec;
    (void) x;
    (void) y;
    (void) z;
    vec[0] = 0.;
    vec[1] = 0.;
    vec[2] = 0.;
    return vec;
}
/* -------------------------- end of hard coded functions --------------------- */




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
            new isInBox{parentCoordinates, newPatchCoords_, layout_.dxdydz()} };

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

    // electricField and magneticField will not be used
    std::unique_ptr<ElectromagInitializer> eminit {
        new ElectromagInitializer{layout_,
                    zeroElectricField,
                    zeroMagneticField, "_EMField", "_EMFields"} };

    std::cout << "creating MLMD ElectromagInitializer" << std::endl;
//    Point origin{0,0,0};

    for (uint32 iComponent=0; iComponent < 3; ++iComponent)
    {

        // ELECTRIC FIELD ----------------
        Field& Ei = eminit->E_.component(iComponent);
        uint32 iStart = layout_.ghostStartIndex(Ei, Direction::X);
        uint32 iEnd   = layout_.ghostEndIndex(  Ei, Direction::X);

//        for (uint32 ix=iStart; ix <= iEnd; ++ix)
//        {
//            Point coord = layout_.fieldNodeCoordinates(Ei, origin, ix, 0, 0);
//            std::array<double,3> E = electricField(coord.x_, origin.y_, origin.z_);
//            Ei(ix) = E[iComponent];
//        }

        // MAGNETIC FIELD ----------------
        Field& Bi = eminit->B_.component(iComponent);
        iStart = layout_.ghostStartIndex(Bi, Direction::X);
        iEnd   = layout_.ghostEndIndex(  Bi, Direction::X);

//        for (uint32 ix=iStart; ix <= iEnd; ++ix)
//        {
//            Point coord = layout_.fieldNodeCoordinates(Bi, origin, ix, 0, 0);
//            std::array<double,3> B = magneticField(coord.x_, origin.y_, origin.z_);
//            Bi(ix) = B[iComponent];
//        }

    }

    return eminit;
}




std::unique_ptr<SolverInitializer> MLMDInitializerFactory::createSolverInitializer() const
{
    return nullptr;
}


std::unique_ptr<OhmInitializer> MLMDInitializerFactory::createOhmInitializer() const
{
    return nullptr;
}


std::unique_ptr<BoundaryCondition> MLMDInitializerFactory::createBoundaryCondition() const
{

    return nullptr;
}



GridLayout const& MLMDInitializerFactory::gridLayout() const
{
    return layout_;
}


Box MLMDInitializerFactory::getBox() const
{
    return layout_.getBox() ;
}


double MLMDInitializerFactory::timeStep() const
{
    return dt_;
}



