#include "Interpolator/particlemesh.h"



/* ----------------------------------------------------------------------------

                      Field interpolations at particles

   ---------------------------------------------------------------------------- */


void fieldAtParticle1D(Interpolator& interp, VecField const& E, VecField const& B,
                       GridLayout const& layout, std::vector<Particle>& particles)
{
    uint32 idirX = static_cast<uint32>(Direction::X);
    uint32 idirY = static_cast<uint32>(Direction::Y);
    uint32 idirZ = static_cast<uint32>(Direction::Z);

    Field const& Ex = E.component(idirX);
    Field const& Ey = E.component(idirY);
    Field const& Ez = E.component(idirZ);

    Field const& Bx = B.component(idirX);
    Field const& By = B.component(idirY);
    Field const& Bz = B.component(idirZ);

    QtyCentering ctrEx_x = layout.fieldCentering(Ex, Direction::X);
    QtyCentering ctrEy_x = layout.fieldCentering(Ey, Direction::X);
    QtyCentering ctrEz_x = layout.fieldCentering(Ez, Direction::X);
    QtyCentering ctrBx_x = layout.fieldCentering(Bx, Direction::X);
    QtyCentering ctrBy_x = layout.fieldCentering(By, Direction::X);
    QtyCentering ctrBz_x = layout.fieldCentering(Bz, Direction::X);


    for (Particle& part : particles)
    {
        part.Ex = interp(part, Ex, ctrEx_x);
        part.Ey = interp(part, Ey, ctrEy_x);
        part.Ez = interp(part, Ez, ctrEz_x);
        part.Bx = interp(part, Bx, ctrBx_x);
        part.By = interp(part, By, ctrBy_x);
        part.Bz = interp(part, Bz, ctrBz_x);
    } // end loop on particles
}



void fieldAtParticle2D(Interpolator& interp, VecField const& E, VecField const& B,
                       GridLayout const& layout, std::vector<Particle>& particles)
{
    uint32 idirX = static_cast<uint32>(Direction::X);
    uint32 idirY = static_cast<uint32>(Direction::Y);
    uint32 idirZ = static_cast<uint32>(Direction::Z);

    Field const& Ex = E.component(idirX);
    Field const& Ey = E.component(idirY);
    Field const& Ez = E.component(idirZ);

    Field const& Bx = B.component(idirX);
    Field const& By = B.component(idirY);
    Field const& Bz = B.component(idirZ);

    QtyCentering ctrEx_x = layout.fieldCentering(Ex, Direction::X);
    QtyCentering ctrEy_x = layout.fieldCentering(Ey, Direction::X);
    QtyCentering ctrEz_x = layout.fieldCentering(Ez, Direction::X);
    QtyCentering ctrBx_x = layout.fieldCentering(Bx, Direction::X);
    QtyCentering ctrBy_x = layout.fieldCentering(By, Direction::X);
    QtyCentering ctrBz_x = layout.fieldCentering(Bz, Direction::X);

    QtyCentering ctrEx_y = layout.fieldCentering(Ex, Direction::Y);
    QtyCentering ctrEy_y = layout.fieldCentering(Ey, Direction::Y);
    QtyCentering ctrEz_y = layout.fieldCentering(Ez, Direction::Y);
    QtyCentering ctrBx_y = layout.fieldCentering(Bx, Direction::Y);
    QtyCentering ctrBy_y = layout.fieldCentering(By, Direction::Y);
    QtyCentering ctrBz_y = layout.fieldCentering(Bz, Direction::Y);


    for (Particle& part : particles)
    {
        part.Ex = interp(part, Ex, ctrEx_x, ctrEx_y);
        part.Ey = interp(part, Ey, ctrEy_x, ctrEy_y);
        part.Ez = interp(part, Ez, ctrEz_x, ctrEz_y);

        part.Bx = interp(part, Bx, ctrBx_x, ctrBx_y);
        part.By = interp(part, By, ctrBy_x, ctrBy_y);
        part.Bz = interp(part, Bz, ctrBz_x, ctrBz_y);
    } // end loop on particles
}



void fieldAtParticle3D(Interpolator& interp, VecField const& E, VecField const& B,
                       GridLayout const& layout, std::vector<Particle>& particles)
{
    uint32 idirX = static_cast<uint32>(Direction::X);
    uint32 idirY = static_cast<uint32>(Direction::Y);
    uint32 idirZ = static_cast<uint32>(Direction::Z);

    Field const& Ex = E.component(idirX);
    Field const& Ey = E.component(idirY);
    Field const& Ez = E.component(idirZ);

    Field const& Bx = B.component(idirX);
    Field const& By = B.component(idirY);
    Field const& Bz = B.component(idirZ);

    QtyCentering ctrEx_x = layout.fieldCentering(Ex, Direction::X);
    QtyCentering ctrEy_x = layout.fieldCentering(Ey, Direction::X);
    QtyCentering ctrEz_x = layout.fieldCentering(Ez, Direction::X);
    QtyCentering ctrBx_x = layout.fieldCentering(Bx, Direction::X);
    QtyCentering ctrBy_x = layout.fieldCentering(By, Direction::X);
    QtyCentering ctrBz_x = layout.fieldCentering(Bz, Direction::X);

    QtyCentering ctrEx_y = layout.fieldCentering(Ex, Direction::Y);
    QtyCentering ctrEy_y = layout.fieldCentering(Ey, Direction::Y);
    QtyCentering ctrEz_y = layout.fieldCentering(Ez, Direction::Y);
    QtyCentering ctrBx_y = layout.fieldCentering(Bx, Direction::Y);
    QtyCentering ctrBy_y = layout.fieldCentering(By, Direction::Y);
    QtyCentering ctrBz_y = layout.fieldCentering(Bz, Direction::Y);

    QtyCentering ctrEx_z = layout.fieldCentering(Ex, Direction::Z);
    QtyCentering ctrEy_z = layout.fieldCentering(Ey, Direction::Z);
    QtyCentering ctrEz_z = layout.fieldCentering(Ez, Direction::Z);
    QtyCentering ctrBx_z = layout.fieldCentering(Bx, Direction::Z);
    QtyCentering ctrBy_z = layout.fieldCentering(By, Direction::Z);
    QtyCentering ctrBz_z = layout.fieldCentering(Bz, Direction::Z);


    for (Particle& part : particles)
    {
        part.Ex = interp(part, Ex, ctrEx_x, ctrEx_y, ctrEx_z);
        part.Ey = interp(part, Ey, ctrEy_x, ctrEy_y, ctrEy_z);
        part.Ez = interp(part, Ez, ctrEz_x, ctrEz_y, ctrEz_z);

        part.Bx = interp(part, Bx, ctrBx_x, ctrBx_y, ctrBx_z);
        part.By = interp(part, By, ctrBy_x, ctrBy_y, ctrBy_z);
        part.Bz = interp(part, Bz, ctrBz_x, ctrBz_y, ctrBz_z);
    } // end loop on particles
}



void fieldsAtParticles(Interpolator& interp, VecField const& E, VecField const& B,
                       GridLayout const& layout, std::vector<Particle>& particles)
{
    switch (layout.nbDimensions())
    {
        case 1: fieldAtParticle1D(interp, E, B, layout, particles); break;
        case 2: fieldAtParticle2D(interp, E, B, layout, particles); break;
        case 3: fieldAtParticle3D(interp, E, B, layout, particles); break;
        default: throw std::runtime_error("fieldsAtParticles - wrong dimensionality");
    }
}



/* ----------------------------------------------------------------------------

                      Interpolations from particles to moments

   ---------------------------------------------------------------------------- */



void update1DChargeDensityAndFlux(Interpolator& interpolator, Species& species,
                                  GridLayout const& layout, std::vector<Particle>& particles)
{
    uint32 idirX = static_cast<uint32>(Direction::X);
    uint32 idirY = static_cast<uint32>(Direction::Y);
    uint32 idirZ = static_cast<uint32>(Direction::Z);

    Field& fx  = species.flux(idirX);
    Field& fy  = species.flux(idirY);
    Field& fz  = species.flux(idirZ);
    Field& rho = species.rho();

    double odx = layout.odx();

    for (const Particle& part : particles)
    {
        interpolator(part, odx, rho, fx, fy, fz, Direction::X);
    }
}


void update2DChargeDensityAndFlux(Interpolator& interpolator, Species& species,
                                  GridLayout const& layout, std::vector<Particle>& particles)
{
    // not implemented function
    // void unused variables
    (void)interpolator;
    (void)species;
    (void)layout;
    (void)particles;
    throw std::runtime_error("NOT IMPLEMENTED");
}


void update3DChargeDensityAndFlux(Interpolator& interpolator, Species& species,
                                  GridLayout const& layout, std::vector<Particle>& particles)
{
    // not implemented function
    // void unused variables
    (void)interpolator;
    (void)species;
    (void)layout;
    (void)particles;
    throw std::runtime_error("NOT IMPLEMENTED");
}


void updateChargeDensityAndFlux(Interpolator& interpolator, Species& species,
                                GridLayout const& layout, std::vector<Particle>& particles)
{
    switch (layout.nbDimensions())
    {
        case 1: update1DChargeDensityAndFlux(interpolator, species, layout, particles); break;
        case 2: update2DChargeDensityAndFlux(interpolator, species, layout, particles); break;
        case 3: update3DChargeDensityAndFlux(interpolator, species, layout, particles); break;
        default: throw std::runtime_error("wrong dimensionality");
    }
}



void compute1DChargeDensityAndFlux(Interpolator& interpolator, Species& species,
                                   GridLayout const& layout, std::vector<Particle>& particles)
{
    species.resetMoments();

    update1DChargeDensityAndFlux(interpolator, species, layout, particles);
}


void compute2DChargeDensityAndFlux(Interpolator& interpolator, Species& species,
                                   GridLayout const& layout, std::vector<Particle>& particles)
{
    species.resetMoments();

    update2DChargeDensityAndFlux(interpolator, species, layout, particles);
}


void compute3DChargeDensityAndFlux(Interpolator& interpolator, Species& species,
                                   GridLayout const& layout, std::vector<Particle>& particles)
{
    species.resetMoments();

    update3DChargeDensityAndFlux(interpolator, species, layout, particles);
}


void computeChargeDensityAndFlux(Interpolator& interpolator, Species& species,
                                 GridLayout const& layout, std::vector<Particle>& particles)
{
    switch (layout.nbDimensions())
    {
        case 1: compute1DChargeDensityAndFlux(interpolator, species, layout, particles); break;
        case 2: compute2DChargeDensityAndFlux(interpolator, species, layout, particles); break;
        case 3: compute3DChargeDensityAndFlux(interpolator, species, layout, particles); break;
        default: throw std::runtime_error("wrong dimensionality");
    }
}
