#ifndef SIMPLEINITIALIZERFACTORY_H
#define SIMPLEINITIALIZERFACTORY_H


#include "initializerfactory.h"


class SimpleInitializerFactory : public InitializerFactory
{
public:
    SimpleInitializerFactory();

    virtual std::unique_ptr<IonsInitializer> createIonsInitializer() const override;
    virtual std::unique_ptr<ElectromagInitializer> createElectromagInitializer() const override;
    virtual std::unique_ptr<OhmInitializer> createOhmInitializer() const override;

    // virtual std::unique_ptr<SolverInitializer> createSolverInitializer() = 0;
    // virtual std::unique_ptr<DiagnosticInitializer> createDiagnosticInitializer() = 0;
    // virtual std::unique_ptr<GridLayoutInitializer> createGridLayoutInitializer() = 0;


private:

    void readInputFile();



};

#endif // SIMPLEINITIALIZERFACTORY_H
