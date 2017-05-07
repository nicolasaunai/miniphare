#ifndef SIMULATIONINITIALIZERFACTORY_H
#define SIMULATIONINITIALIZERFACTORY_H

#include "Initializer/diagnosticinitializer.h"
#include "Initializer/initializerfactory.h"
#include "Time/time.h"

class SimulationInitializerFactory : public InitializerFactory
{
public:
    virtual std::unique_ptr<DiagnosticInitializer> createDiagnosticInitializer() const = 0;
    virtual std::unique_ptr<Time> createTimeManager() const                            = 0;
};




std::unique_ptr<SimulationInitializerFactory> fromCommandLine(int argc, char** argv);




#endif // SIMULATIONINITIALIZERFACTORY_H
