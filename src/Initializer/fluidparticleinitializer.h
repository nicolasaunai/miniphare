#ifndef FLUIDPARTICLEINITIALIZER_H
#define FLUIDPARTICLEINITIALIZER_H

#include <random>

#include "grid/gridlayout.h"
#include "particleinitializer.h"




/**
 * @brief FluidParticleInitializer is a ParticleInitializer where
 *        particles are created following density, bulk velocity and temperature
 *        profiles and are distributed according to a locally Mawwellian
 *        distribution function.
 */
class FluidParticleInitializer : public ParticleInitializer
{


private:

    GridLayout     layout_;
    ScalarFunction density;
    VectorFunction bulkVelocity;
    ScalarFunction thermalSpeed;
    double particleCharge_;

    uint32 nbrParticlePerCell_;

    void loadParticles1D_(std::vector<Particle>& particles) const;
    void loadParticles2D_(std::vector<Particle>& particles) const;
    void loadParticles3D_(std::vector<Particle>& particles) const;

public:


    FluidParticleInitializer(GridLayout const& layout,
                             ScalarFunction densityProfile,
                             VectorFunction bulkVelocityProfile,
                             ScalarFunction thermalSpeedProfile,
                             uint32 nbrPartPerCell,
                             double particleCharge);

    virtual void loadParticles(std::vector<Particle>& particles) const override;

    virtual ~FluidParticleInitializer();

};





#endif // FLUIDPARTICLEINITIALIZER_H
