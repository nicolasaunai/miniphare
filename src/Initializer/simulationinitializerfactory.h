#ifndef SIMULATIONINITIALIZERFACTORY_H
#define SIMULATIONINITIALIZERFACTORY_H

#include "Initializer/initializerfactory.h"
#include "Initializer/diagnosticinitializer.h"


class SimulationInitializerFactory : public InitializerFactory
{
public:
    virtual std::unique_ptr<DiagnosticInitializer> createDiagnosticInitializer() const = 0;
};





std::unique_ptr<SimulationInitializerFactory> fromCommandLine(int argc, char** argv);





#endif // SIMULATIONINITIALIZERFACTORY_H
