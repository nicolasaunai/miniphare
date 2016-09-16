#include "initializerfactory.h"
#include "pythoninitializerfactory.h"
#include "simpleinitializerfactory.h"


InitializerFactory::~InitializerFactory() {}



std::unique_ptr<InitializerFactory> fromCommandLine(int argc, char** argv)
{
    // hard coded
    return std::unique_ptr<InitializerFactory> { new SimpleInitializerFactory{} };
}
