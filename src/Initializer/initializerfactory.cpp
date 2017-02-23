#include "initializerfactory.h"
#include "pythoninitializerfactory.h"
#include "simpleinitializerfactory.h"





/**
\fn std::unique_ptr<InitializerFactory> fromCommandLine(int argc, char** argv)
\brief returns a pointer to a concrete InitializerFactory from command line arguments


e.g. if ./miniphare init.py is ran, a pointer to a PythonInitializerFactory object
is returned*/
std::unique_ptr<InitializerFactory> fromCommandLine(int argc, char** argv)
{
    // hard coded
    return std::unique_ptr<InitializerFactory> { new SimpleInitializerFactory{} };
}
