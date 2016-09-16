#include "particleinitializer.h"
#include "particleinitializerimpfluid.h"




ParticleInitializer::ParticleInitializer(GridLayout const& layout, ParticleInitializerType initType)
    :implPtr_ {nullptr}
{
    switch ( initType )
    {
        case ParticleInitializerType::Fluid:
            implPtr_.reset( new ParticleInitializerImplFluid(layout) );

        break;
    }

}



ParticleInitializer::ParticleInitializer(ParticleInitializer const& source)
    : implPtr_ { source.implPtr_->clone()}
{

}

