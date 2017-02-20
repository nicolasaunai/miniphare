
#include "mlmdinitializerfactory.h"
#include "mlmdparticleinitializer.h"
#include "utility.h"


std::unique_ptr<IonsInitializer> MLMDInitializerFactory::createIonsInitializer() const
{
    /* this routine creates an ion initializer with a Patch Choice function. */
    std::unique_ptr<IonsInitializer> ionInitPtr{ new IonsInitializer{} };
    Ions const& parentIons = parentPatch_->ions();

    for (uint32 ispe=0; ispe < parentIons.nbrSpecies(); ++ispe)
    {
        Species const& species = parentIons.species(ispe);
        Box  patchCoordinates  = parentPatch_->coordinates();
        std::unique_ptr<ParticleSelector> selector{ new isInBox{patchCoordinates} };
        std::unique_ptr<ParticleInitializer> particleInit{new MLMDParticleInitializer{species,
                                                    std::move(selector) }};
        ionInitPtr->masses.push_back( parentIons.species(ispe).mass() );
        ionInitPtr->particleInitializers.push_back( std::move(particleInit) );
    }




    return nullptr;
}




std::unique_ptr<ElectromagInitializer>
MLMDInitializerFactory::createElectromagInitializer() const
{

    return nullptr;
}


std::unique_ptr<SolverInitializer> MLMDInitializerFactory::createSolverInitializer() const
{
    return nullptr;
}


std::unique_ptr<OhmInitializer> MLMDInitializerFactory::createOhmInitializer() const
{
    return nullptr;
}


std::unique_ptr<BoundaryCondition> MLMDInitializerFactory::createBoundaryCondition() const
{

    return nullptr;
}



