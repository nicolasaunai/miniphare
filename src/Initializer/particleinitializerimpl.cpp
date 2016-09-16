#include "particleinitializerimpl.h"

ParticleInitializerImpl::ParticleInitializerImpl()
{

}



std::unique_ptr<ParticleInitializerImpl> ParticleInitializerImpl::clone() const
{
    return std::unique_ptr<ParticleInitializerImpl> { cloneImpl() };
}
