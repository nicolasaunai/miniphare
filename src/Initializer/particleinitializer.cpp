#include "particleinitializer.h"
#include "fluidparticleinitializer.h"



#if 0
//TODO clone method should be defined for this abstract type.

std::unique_ptr<ParticleInitializer> ParticleInitializer::clone() const
{
     return std::unique_ptr<ParticleInitializer> {cloneImpl()};
}
#endif


ParticleInitializer::~ParticleInitializer()
{
}
