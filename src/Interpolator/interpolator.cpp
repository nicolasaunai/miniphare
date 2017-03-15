
#include <stdexcept>

#include "grid/gridlayoutdefs.h"
#include "Interpolator/interpolator.h"
#include "IndexesAndWeights/indexesandweightso1.h"
#include "IndexesAndWeights/indexesandweightso2.h"
#include "IndexesAndWeights/indexesandweightso3.h"
#include "IndexesAndWeights/indexesandweightso4.h"



Interpolator::Interpolator(uint32 order)
    : impl_{ nullptr}
{

    switch( order )
    {
    case 1:
        impl_ = std::unique_ptr<IndexesAndWeightsO1>( new IndexesAndWeightsO1(order) ) ;
        break;
    case 2:
        impl_ = std::unique_ptr<IndexesAndWeightsO2>( new IndexesAndWeightsO2(order) ) ;
        break;
    case 3:
        impl_ = std::unique_ptr<IndexesAndWeightsO3>( new IndexesAndWeightsO3(order) ) ;
        break;
    case 4:
        impl_ = std::unique_ptr<IndexesAndWeightsO4>( new IndexesAndWeightsO4(order) ) ;
        break;
    }
}







/* ----------------------------------------------------------------------------

                      Field interpolations at particles

   ---------------------------------------------------------------------------- */





void fieldAtParticle1D(Interpolator const& interp,
                       VecField const & E , VecField const & B,
                       GridLayout const & layout,
                       std::vector<Particle>& particles)
{
    uint32 idirX = static_cast<uint32>(Direction::X) ;
    uint32 idirY = static_cast<uint32>(Direction::Y) ;
    uint32 idirZ = static_cast<uint32>(Direction::Z) ;

    Field const & Ex = E.component(idirX) ;
    Field const & Ey = E.component(idirY) ;
    Field const & Ez = E.component(idirZ) ;

    Field const & Bx = B.component(idirX) ;
    Field const & By = B.component(idirY) ;
    Field const & Bz = B.component(idirZ) ;

    std::vector<std::reference_wrapper<Field const>> ExyzBxyzFields \
            = {Ex, Ey, Ez, Bx, By, Bz} ;

    for (uint32 iPart=0; iPart < particles.size(); ++iPart)
    {
        Particle& part = particles[iPart];

        part.Ex = 0. ; part.Ey = 0. ; part.Ez = 0. ;
        part.Bx = 0. ; part.By = 0. ; part.Bz = 0. ;

        std::vector<std::reference_wrapper<double>> partFields = \
        {part.Ex, part.Ey, part.Ez, part.Bx, part.By, part.Bz} ;

        for(uint32 ifield=0 ; ifield<partFields.size() ; ++ifield )
        {
            double & particleField = partFields[ifield] ;
            Field const& meshField = ExyzBxyzFields[ifield] ;
            auto centering = layout.fieldCentering( meshField, Direction::X ) ;
            particleField =  interp(part, meshField, Direction::X, centering);

        }// end loop on fields
    }// end loop on particles
}



void fieldAtParticle2D(Interpolator const& interp,
                       VecField const & E , VecField const & B,
                       GridLayout const & layout,
                       std::vector<Particle>& particles)
{
    // not implemented function
    // void unused variables
    (void) interp;
    (void) E;
    (void) B;
    (void) layout;
    (void)particles;
    throw std::runtime_error("NOT IMPLEMENTED");
}


void fieldAtParticle3D(Interpolator const& interp,
                       VecField const & E , VecField const & B,
                       GridLayout const & layout,
                       std::vector<Particle>& particles)
{
    // not implemented function
    // void unused variables
    (void) interp;
    (void) E;
    (void) B;
    (void) layout;
    (void)particles;
    throw std::runtime_error("NOT IMPLEMENTED");
}





void fieldsAtParticles(Interpolator const& interp,
                       VecField const& E, VecField const& B,
                       GridLayout const& layout,
                       std::vector<Particle>& particles)
{
    switch (layout.nbDimensions())
    {
    case 1:
        fieldAtParticle1D(interp, E, B, layout, particles);
        break;
    case 2:
        fieldAtParticle2D(interp, E, B, layout, particles);
        break;
    case 3:
        fieldAtParticle3D(interp, E, B, layout, particles);
        break;
    default:
        throw std::runtime_error("wrong dimensionality");
    }
}





/* ----------------------------------------------------------------------------

                      Interpolations from particles to moments

   ---------------------------------------------------------------------------- */









void compute1DChargeDensityAndFlux(Interpolator& interpolator,
                                  Species& species,
                                  GridLayout const& layout,
                                  std::vector<Particle>& particles)
{
    uint32 idirX = static_cast<uint32>(Direction::X) ;
    uint32 idirY = static_cast<uint32>(Direction::Y) ;
    uint32 idirZ = static_cast<uint32>(Direction::Z) ;

    Field& fx  = species.flux(idirX) ;
    Field& fy  = species.flux(idirY) ;
    Field& fz  = species.flux(idirZ) ;
    Field& rho = species.rho();

    species.resetMoments() ;

    for( const Particle & part : particles)
    {
        interpolator(part, layout.dx(), rho, fx, fy, fz, Direction::X);
    }

}





void compute2DChargeDensityAndFlux(Interpolator& interpolator,
                                  Species& species,
                                  GridLayout const& layout,
                                  std::vector<Particle>& particles)
{
    // not implemented function
    // void unused variables
    (void) interpolator;
    (void) species;
    (void) layout;
    (void) particles;
    throw std::runtime_error("NOT IMPLEMENTED");
}


void compute3DChargeDensityAndFlux(Interpolator& interpolator,
                                  Species& species,
                                  GridLayout const& layout,
                                  std::vector<Particle>& particles)
{
    // not implemented function
    // void unused variables
    (void) interpolator;
    (void) species;
    (void) layout;
    (void) particles;
    throw std::runtime_error("NOT IMPLEMENTED");
}


void computeChargeDensityAndFlux(Interpolator& interpolator,
                                 Species& species,
                                 GridLayout const& layout,
                                 std::vector<Particle>& particles)
{
    switch (layout.nbDimensions())
    {
    case 1:
        compute1DChargeDensityAndFlux(interpolator, species, layout, particles);
        break;
    case 2:
        compute2DChargeDensityAndFlux(interpolator, species, layout, particles);
        break;
    case 3:
        compute3DChargeDensityAndFlux(interpolator, species, layout, particles);
        break;
    default:
        throw std::runtime_error("wrong dimensionality");
    }
}










