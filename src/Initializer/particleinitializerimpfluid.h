#ifndef FLUIDPARTICLEINITIALIZER_H
#define FLUIDPARTICLEINITIALIZER_H


#include "grid/gridlayout.h"
#include "particleinitializerimpl.h"




/**
 * @brief FluidParticleInitializer is a ParticleInitializer where
 *        particles are created following density, bulk velocity and temperature
 *        profiles and are distributed according to a locally Mawwellian
 *        distribution function.
 */
class ParticleInitializerImplFluid : public ParticleInitializerImpl
{

private:

    GridLayout layout_;
    virtual ParticleInitializerImplFluid* cloneImpl() const override;

public:

    ParticleInitializerImplFluid(GridLayout const& layout):layout_{layout} {}

    // ParticleInitializerImplFluid is always manipulated through the
    // particleInitializerImpl interface. Therefore it does not
    // need a proper public clone() method. It just needs to implement
    // the particleInitializerImpl::cloneImpl method.

    virtual void loadParticles(std::vector<Particle>& particles) const override;

    virtual ~ParticleInitializerImplFluid();

    ScalarFunction density;
    VectorFunction bulkVelocity;
    ScalarFunction temperature;

};





#endif // FLUIDPARTICLEINITIALIZER_H
