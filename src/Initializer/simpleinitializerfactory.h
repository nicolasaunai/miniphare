#ifndef SIMPLEINITIALIZERFACTORY_H
#define SIMPLEINITIALIZERFACTORY_H


#include "initializerfactory.h"



double densityProton1(double x, double y, double z) ;
double densityProton2(double x, double y, double z) ;

double thermalSpeedProton1(double x, double y, double z) ;
double thermalSpeedProton2(double x, double y, double z) ;

std::array<double,3> bulkVelocityProton1(double x, double y, double z) ;
std::array<double,3> bulkVelocityProton2(double x, double y, double z) ;

std::array<double,3>  magneticField(double x, double y, double z) ;
std::array<double,3> electricField(double x, double y, double z) ;


/**
 * @brief The SimpleInitializerFactory class is a concrete InitializerFactory
 * used to simply initialize miniphare with hard-coded parameters. It is mainly
 * used for development purposes. The SumpleInitializerFactory will create
 * ions etc. from the FluidParticleInitializer
 */
class SimpleInitializerFactory : public InitializerFactory
{
public:
    // TODO : see what Ctors will need to be defined
    SimpleInitializerFactory();

    virtual std::unique_ptr<IonsInitializer> createIonsInitializer() const override;
    virtual std::unique_ptr<ElectromagInitializer> createElectromagInitializer() const override;
    virtual std::unique_ptr<OhmInitializer> createOhmInitializer() const override;
    virtual std::unique_ptr<SolverInitializer> createSolverInitializer() const override;
    virtual std::unique_ptr<BoundaryCondition> createBoundaryCondition() const override;

    virtual Box getRefinedBox() const override;
    virtual GridLayout const& gridLayout() const override;
    virtual double timeStep() const override;

    // virtual std::unique_ptr<DiagnosticInitializer> createDiagnosticInitializer() = 0;
    // virtual std::unique_ptr<GridLayoutInitializer> createGridLayoutInitializer() = 0;

private:

    GridLayout layout_;
    double dt_;
    void readInputFile();



};



#endif // SIMPLEINITIALIZERFACTORY_H


