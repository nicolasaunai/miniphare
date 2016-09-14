#ifndef PARTICLEINITIALIZER_H
#define PARTICLEINITIALIZER_H


#include "Plasmas/particles.h"


/**
 * @brief ParticleInitializer is an interface class used by Species to
 *        load their particles into their internal particle array without
 *        knowing how it is done. Loading particles is typically done at
 *        initialization of Species, either at the start of a simulation
 *        or when a new Patch is created by the MLMD system.
 */
class ParticleInitializer
{

public:

    virtual std::unique_ptr<ParticleInitializer> clone() const = 0;

    virtual void loadParticles(std::vector<Particle>& particles) const = 0;

    virtual ~ParticleInitializer();

};




#endif // PARTICLEINITIALIZER_H
