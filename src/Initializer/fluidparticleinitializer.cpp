#include "fluidparticleinitializer.h"



FluidParticleInitializer::FluidParticleInitializer()
{

}

/**
 * @brief clone a FluidParticleInitializer
 */
 std::unique_ptr<ParticleInitializer> FluidParticleInitializer::clone() const
 {
     return nullptr;
 }


 /**
  * @brief FluidParticleInitializer::loadParticles loads an array of Particle
  * following density, velocity and temperature profiels and a local
  * Maxwellian distirbution function
  * @param particles is the vector that will be filled with particles
  */
 void FluidParticleInitializer::loadParticles(std::vector<Particle>& particles) const
 {

 }


 FluidParticleInitializer::~FluidParticleInitializer()
 {

 }
