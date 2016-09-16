#include "particleinitializerimpfluid.h"



ParticleInitializerImplFluid* ParticleInitializerImplFluid::cloneImpl() const
{
    return new ParticleInitializerImplFluid(*this);
}



 /**
  * @brief FluidParticleInitializer::loadParticles loads an array of Particle
  * following density, velocity and temperature profiels and a local
  * Maxwellian distirbution function
  * @param particles is the vector that will be filled with particles
  */
 void ParticleInitializerImplFluid::loadParticles(std::vector<Particle>& particles) const
 {

 }


 ParticleInitializerImplFluid::~ParticleInitializerImplFluid()
 {

 }
