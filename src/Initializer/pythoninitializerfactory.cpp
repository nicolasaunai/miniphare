#include "pythoninitializerfactory.h"


std::unique_ptr<IonsInitializer> PythonInitializerFactory::createIonsInitializer() const
{
    return nullptr;
}


std::unique_ptr<ElectromagInitializer> PythonInitializerFactory::createElectromagInitializer() const
{
    return nullptr;
}


std::unique_ptr<OhmInitializer> PythonInitializerFactory::createOhmInitializer() const
{
    return nullptr;
}
