#ifndef INITIALIZERFACTORY_H
#define INITIALIZERFACTORY_H


#include "memory"
#include "ionsinitializer.h"
#include "electromaginitializer.h"
#include "ohminitializer.h"


/**
 * @brief An InitializerFactory is used as an interface to build and initialize
 *       objects in the simulation.
 *
 *
 * The InitializerFactory follows the Abstract Factory pattern so that
 * objects can be built without explicitly depending on the way the code
 * gets its input parameters. We consider for now one type of Initializer:
 *
 */
class InitializerFactory
{

public:

    virtual std::unique_ptr<IonsInitializer> createIonsInitializer() const = 0;
    virtual std::unique_ptr<ElectromagInitializer> createElectromagInitializer() const = 0;
    virtual std::unique_ptr<OhmInitializer> createOhmInitializer() const = 0;
    virtual GridLayout const& gridLayout() const = 0;
    virtual double timeStep() const = 0;

    // virtual std::unique_ptr<SolverInitializer> createSolverInitializer() = 0;
    // virtual std::unique_ptr<DiagnosticInitializer> createDiagnosticInitializer() = 0;
    // virtual std::unique_ptr<GridLayoutInitializer> createGridLayoutInitializer() = 0;


    virtual ~InitializerFactory() = 0;

};





std::unique_ptr<InitializerFactory> fromCommandLine(int argc, char** argv);




#endif // INITIALIZERFACTORY_H
