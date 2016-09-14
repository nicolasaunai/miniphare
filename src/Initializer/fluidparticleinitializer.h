#ifndef FLUIDPARTICLEINITIALIZER_H
#define FLUIDPARTICLEINITIALIZER_H


#include "particleinitializer.h"


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
