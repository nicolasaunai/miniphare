#include "particleinitializer.h"



ParticleInitializer::ParticleInitializer(ParticleInitializer const& source)
    : implPtr_ { source.implPtr_->clone()}
{

}

