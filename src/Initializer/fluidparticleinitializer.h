#ifndef FLUIDPARTICLEINITIALIZER_H
#define FLUIDPARTICLEINITIALIZER_H


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

    using Function = double (*) (double x, double y, double z);

public:

    FluidParticleInitializer();

    virtual std::unique_ptr<ParticleInitializer> clone() const override;

    virtual void loadParticles(std::vector<Particle>& particles) const override;

    virtual ~FluidParticleInitializer();

    Function density;
    Function bulkVelocity;
    Function temperature;

};




#endif // FLUIDPARTICLEINITIALIZER_H
