#include "particleinitializerimpl.h"

ParticleInitializerImpl::ParticleInitializerImpl()
{

}

#if 0

std::unique_ptr<ParticleInitializerImpl> ParticleInitializerImpl::clone() const
{
    return std::unique_ptr<ParticleInitializerImpl> { cloneImpl() };
}

#endif
