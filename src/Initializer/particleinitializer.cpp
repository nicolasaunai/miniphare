#include "particleinitializer.h"
#include "fluidparticleinitializer.h"



/*
ParticleInitializer::ParticleInitializer(GridLayout const& layout, ParticleInitializerType initType)
    :implPtr_ {nullptr}
{
    switch ( initType )
    {
        case ParticleInitializerType::Fluid:
            implPtr_.reset( new FluidParticleInitializer(layout) );

        break;
    }
}
*/

/*
ParticleInitializer::ParticleInitializer(ParticleInitializer const& source)
    : implPtr_ { source.implPtr_->clone()}
{

}
*/

#if 0

std::unique_ptr<ParticleInitializer> ParticleInitializer::clone() const
{
     return std::unique_ptr<ParticleInitializer> {cloneImpl()};
}
#endif


ParticleInitializer::~ParticleInitializer()
{

}







