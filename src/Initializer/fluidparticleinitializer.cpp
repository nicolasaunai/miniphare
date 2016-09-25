#include "fluidparticleinitializer.h"
#include <iostream>



FluidParticleInitializer::FluidParticleInitializer(GridLayout const& layout)
    : ParticleInitializer {},
      layout_{layout}
{

}



 /**
  * @brief FluidParticleInitializer::loadParticles loads an array of Particle
  * following density, velocity and temperature profiles and a local
  * Maxwellian distirbution function
  * @param particles is the vector that will be filled with particles
  */
 void FluidParticleInitializer::loadParticles(std::vector<Particle>& particles) const
 {
    std::cout << "I'm loading particles with Fluid profiles" << std::endl;
 }


 FluidParticleInitializer::~FluidParticleInitializer()
 {

 }
