#include "Interpolator/particlemesh.h"



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

    QtyCentering centeringEx = layout.fieldCentering(Ex, Direction::X ) ;
    QtyCentering centeringEy = layout.fieldCentering(Ey, Direction::X ) ;
    QtyCentering centeringEz = layout.fieldCentering(Ez, Direction::X ) ;
    QtyCentering centeringBx = layout.fieldCentering(Bx, Direction::X ) ;
    QtyCentering centeringBy = layout.fieldCentering(By, Direction::X ) ;
    QtyCentering centeringBz = layout.fieldCentering(Bz, Direction::X ) ;


    for (uint32 iPart=0; iPart < particles.size(); ++iPart)
    {
        Particle& part = particles[iPart];

        part.Ex = interp(part, Ex, Direction::X, centeringEx);
        part.Ey = interp(part, Ey, Direction::X, centeringEy);
        part.Ez = interp(part, Ez, Direction::X, centeringEz);

        part.Bx = interp(part, Bx, Direction::X, centeringBx);
        part.By = interp(part, By, Direction::X, centeringBy);
        part.Bz = interp(part, Bz, Direction::X, centeringBz);
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
    double odx = layout.odx();

    for( const Particle & part : particles)
    {
        interpolator(part, odx, rho, fx, fy, fz, Direction::X);
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



