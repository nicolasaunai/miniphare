#ifndef SIMPLEPARTICLEINITIALIZER_H
#define SIMPLEPARTICLEINITIALIZER_H

#include "grid/gridlayout.h"
#include "particleinitializer.h"




/**
 * @brief FluidParticleInitializer is a ParticleInitializer where
 *        particles are created following density, bulk velocity and temperature
 *        profiles and are distributed according to a locally Mawwellian
 *        distribution function.
 */
class SimpleParticleInitializer : public ParticleInitializer
{

private:

    GridLayout layout_;

public:

    explicit SimpleParticleInitializer(GridLayout const& layout);

    virtual void loadParticles(std::vector<Particle>& particles) const override;

    virtual ~SimpleParticleInitializer() {}

    ScalarFunction density;
    VectorFunction bulkVelocity;
    ScalarFunction temperature;

};




#endif // SIMPLEPARTICLEINITIALIZER_H
