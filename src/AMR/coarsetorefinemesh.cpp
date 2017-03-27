
#include "AMR/coarsetorefinemesh.h"




/* ----------------------------------------------------------------------------

                      Field interpolations at refined nodes

   ---------------------------------------------------------------------------- */



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

    std::array<std::reference_wrapper<Field const>, 6>
            ExyzBxyzCoarse = { {Ex, Ey, Ez, Bx, By, Bz} } ;


    Field & ExNew = Erefined.component(idirX) ;
    Field & EyNew = Erefined.component(idirY) ;
    Field & EzNew = Erefined.component(idirZ) ;

    Field & BxNew = Brefined.component(idirX) ;
    Field & ByNew = Brefined.component(idirY) ;
    Field & BzNew = Brefined.component(idirZ) ;

    std::array<std::reference_wrapper<Field>, 6>
            ExyzBxyzRefined = { { ExNew, EyNew, EzNew, BxNew, ByNew, BzNew} } ;


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

        // we might interpolate the parent field
        // from a primal or a dual mesh
        auto centering = coarseLayout.fieldCentering( coarseField, Direction::X ) ;

        // loop on new field indexes
        for( uint32 ix=iStart ; ix<=iEnd ; ++ix )
        {
            // ix coordinate can be seen as the Particle position of
            // the method fieldAtParticle1D(...)
            //
            // Compute the reduced coordinate
            // on the parent GridLayout
            double delta = ix * refinedLayout.dx() / coarseLayout.dx() ;

            // coord is a reduced coordinate on the parent GridLayout
            double coord = newOriginReducedOnCoarse + delta ;

            // nodes from the parent layout now contribute
            // to the node: ix, located on the new layout
            refinedField(ix) = interp( coord, coarseField, centering ) ;
        }
    }


}



void fieldAtRefinedNodes2D(Interpolator const& interp,
                           GridLayout const & coarseLayout,
                           VecField const & Ecoarse , VecField const & Bcoarse,
                           GridLayout const & refinedLayout,
                           VecField & Erefined , VecField & Brefined)
{
    (void) interp ;
    (void) coarseLayout ;
    (void) Ecoarse ;
    (void) Bcoarse ;
    (void) refinedLayout ;
    (void) Erefined ;
    (void) Brefined ;
    throw std::runtime_error("NOT IMPLEMENTED");
}



void fieldAtRefinedNodes3D(Interpolator const& interp,
                           GridLayout const & coarseLayout,
                           VecField const & Ecoarse , VecField const & Bcoarse,
                           GridLayout const & refinedLayout,
                           VecField & Erefined , VecField & Brefined)
{
    (void) interp ;
    (void) coarseLayout ;
    (void) Ecoarse ;
    (void) Bcoarse ;
    (void) refinedLayout ;
    (void) Erefined ;
    (void) Brefined ;
    throw std::runtime_error("NOT IMPLEMENTED");
}


void fieldAtRefinedNodes( Interpolator const& interpolator,
                          GridLayout const & coarseLayout,
                          Electromag const & parentElectromag ,
                          GridLayout const & refinedLayout,
                          ElectromagInitializer & eminit )
{

    switch( coarseLayout.nbDimensions() )
    {
    case 1:
        fieldAtRefinedNodes1D( interpolator,
                               coarseLayout,
                               parentElectromag.getE() , parentElectromag.getB(),
                               refinedLayout,
                               eminit.E_ , eminit.B_ ) ;
        break;
    case 2:
        fieldAtRefinedNodes2D( interpolator,
                               coarseLayout,
                               parentElectromag.getE() , parentElectromag.getB(),
                               refinedLayout,
                               eminit.E_ , eminit.B_ ) ;
        break;
    case 3:
        fieldAtRefinedNodes3D( interpolator,
                               coarseLayout,
                               parentElectromag.getE() , parentElectromag.getB(),
                               refinedLayout,
                               eminit.E_ , eminit.B_ ) ;
        break;
    default:
        throw std::runtime_error("wrong dimensionality");
    }

}


