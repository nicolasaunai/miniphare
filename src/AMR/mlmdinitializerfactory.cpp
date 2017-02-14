
#include "mlmdinitializerfactory.h"



std::unique_ptr<IonsInitializer> MLMDInitializerFactory::createIonsInitializer() const
{
    /*
        this routine creates an ion initializer with a Patch Choice function.
    */

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





