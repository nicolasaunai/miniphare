#include "simpleinitializerfactory.h"
#include "Initializer/fluidparticleinitializer.h"


SimpleInitializerFactory::SimpleInitializerFactory()
{

}




std::unique_ptr<IonsInitializer> SimpleInitializerFactory::createIonsInitializer() const
{
    const uint32 nbrSpecies = 2;

    // should be obtained from
    // the factory somehow...
    GridLayout gl{ {0.1,0.,0.}, {42, 0, 0}, 1, "yee" };

    std::unique_ptr<IonsInitializer> ionInitPtr{ new IonsInitializer{} };

    ionInitPtr->masses.push_back(1.);
    ionInitPtr->masses.push_back(1.);
    ionInitPtr->names.push_back("proton1");
    ionInitPtr->names.push_back("proton2");

    // those rvalues should be moved so ParticleInitializer
    // should have noexcept move Ctor.
    ionInitPtr->particleInitializers.push_back( std::unique_ptr<ParticleInitializer>
                                                    {new FluidParticleInitializer{gl} } );

    ionInitPtr->particleInitializers.push_back( std::unique_ptr<ParticleInitializer>
                                                    {new FluidParticleInitializer{gl} } );

    return ionInitPtr;

}





std::unique_ptr<ElectromagInitializer> SimpleInitializerFactory::createElectromagInitializer() const
{
    return nullptr;
}





std::unique_ptr<OhmInitializer> SimpleInitializerFactory::createOhmInitializer() const
{
    return nullptr;
}


