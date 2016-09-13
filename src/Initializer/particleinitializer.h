#ifndef PARTICLEINITIALIZER_H
#define PARTICLEINITIALIZER_H


#include "Plasmas/particles.h"


class ParticleInitializer
{

public:

    ParticleInitializer();

    virtual void loadParticles(std::vector<Particle>& particles) const = 0;

    virtual ~ParticleInitializer();

};

#endif // PARTICLEINITIALIZER_H
