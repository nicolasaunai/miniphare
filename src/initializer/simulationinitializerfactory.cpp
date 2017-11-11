
#include <memory>
#include <string>

#include "pythoninitializerfactory.h"
#include "simpleinitializerfactory.h"
#include "simulationinitializerfactory.h"
#include <initializer/ascii_initializer/asciiinitializerfactory.h>




/**
\fn std::unique_ptr<InitializerFactory> fromCommandLine(int argc, char** argv)
\brief returns a pointer to a concrete InitializerFactory from command line arguments


e.g. if ./miniphare init.py is ran, a pointer to a PythonInitializerFactory object
is returned*/
std::unique_ptr<SimulationInitializerFactory> fromCommandLine(int argc, char** argv)
{
    using simuFactoPtr = std::unique_ptr<SimulationInitializerFactory>;
    auto factory       = simuFactoPtr{nullptr};

    switch (argc)
    {
        case 1: factory = simuFactoPtr{new SimpleInitializerFactory{}}; break;

        case 2:
            std::string arg = argv[1];
            if (arg.substr(arg.find_last_of(".") + 1) == "ini")
            {
                std::cout << "INI input detected, building INI factory...\n";
                factory = simuFactoPtr{new AsciiInitializerFactory{argv[1]}};
            }

            break;
    }

    return factory;
}
