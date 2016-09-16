#ifndef PARTICLEINITIALIZERIMPL_H
#define PARTICLEINITIALIZERIMPL_H


#include <memory>
#include <vector>

#include "Plasmas/particles.h"


class ParticleInitializerImpl
{
public:
    ParticleInitializerImpl();

    std::unique_ptr<ParticleInitializerImpl> clone() const;

    // virtual void loadParticles(std::vector<Particle>& particles) const = 0;

private:

   //;


};

#endif // PARTICLEINITIALIZERIMPL_H
