#include "simpleinitializerfactory.h"



SimpleInitializerFactory::SimpleInitializerFactory()
{

}



std::unique_ptr<IonsInitializer> SimpleInitializerFactory::createIonsInitializer() const
{
    const uint32 nbrSpecies = 2;
    std::unique_ptr<IonsInitializer> ionInitPtr{ new IonsInitializer{nbrSpecies} };


    //ionInitPtr->masses_ = {1.,1.};

    std::vector<double> masses{1., 1.};
    std::vector<std::string> names{"proton1", "proton2"};
    //std::vector<ParticleInitializer>


}


std::unique_ptr<ElectromagInitializer> SimpleInitializerFactory::createElectromagInitializer() const
{
    return nullptr;
}





std::unique_ptr<OhmInitializer> SimpleInitializerFactory::createOhmInitializer() const
{
    return nullptr;
}


