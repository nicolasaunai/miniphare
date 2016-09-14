#include "fluidparticleinitializer.h"



FluidParticleInitializer::FluidParticleInitializer()
{

}


 std::unique_ptr<ParticleInitializer> FluidParticleInitializer::clone() const
 {
     return nullptr;
 }



 void FluidParticleInitializer::loadParticles(std::vector<Particle>& particles) const
 {

 }


 FluidParticleInitializer::~FluidParticleInitializer()
 {

 }
