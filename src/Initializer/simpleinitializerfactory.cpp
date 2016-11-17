#include "simpleinitializerfactory.h"
#include "Initializer/fluidparticleinitializer.h"


SimpleInitializerFactory::SimpleInitializerFactory()
    : layout_{ {0.1,0.,0.}, {42, 0, 0}, 1, "yee", 2}, // hard-coded... will come from input somehow
      dt_{0.1}
{

}





std::unique_ptr<IonsInitializer> SimpleInitializerFactory::createIonsInitializer() const
{
    const uint32 nbrSpecies = 2;

    // should be obtained from
    // the factory somehow...

    std::unique_ptr<IonsInitializer> ionInitPtr{ new IonsInitializer{} };

    ionInitPtr->nbrSpecies = nbrSpecies;
    ionInitPtr->masses.push_back(1.);
    ionInitPtr->masses.push_back(1.);
    ionInitPtr->names.push_back("proton1");
    ionInitPtr->names.push_back("proton2");

    // TODO those rvalues should be moved so ParticleInitializer should have noexcept move Ctor.
    ionInitPtr->particleInitializers.push_back( std::unique_ptr<ParticleInitializer>
                                                    {new FluidParticleInitializer{layout_} } );

    ionInitPtr->particleInitializers.push_back( std::unique_ptr<ParticleInitializer>
                                                    {new FluidParticleInitializer{layout_} } );

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

std::string SimpleInitializerFactory::pusherType() const
{
    std::string pusher = "modifiedBoris" ;

    return pusher ;
}

GridLayout const& SimpleInitializerFactory::gridLayout() const
{
    return layout_;
}


double SimpleInitializerFactory::timeStep() const
{
    return dt_;
}
