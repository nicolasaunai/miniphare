#ifndef FLUIDPARTICLEINITIALIZER_H
#define FLUIDPARTICLEINITIALIZER_H


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

    GridLayout layout_;
    //virtual FluidParticleInitializer* cloneImpl() const override;

public:

    FluidParticleInitializer(GridLayout const& layout);

    // ParticleInitializerImplFluid is always manipulated through the
    // particleInitializerImpl interface. Therefore it does not
    // need a proper public clone() method. It just needs to implement
    // the particleInitializerImpl::cloneImpl method.

    virtual void loadParticles(std::vector<Particle>& particles) const override;

    virtual ~FluidParticleInitializer();

    ScalarFunction density;
    VectorFunction bulkVelocity;
    ScalarFunction temperature;

};





#endif // FLUIDPARTICLEINITIALIZER_H
