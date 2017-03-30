#ifndef SIMPLEINITIALIZERFACTORY_H
#define SIMPLEINITIALIZERFACTORY_H


#include "initializerfactory.h"




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
    SimpleInitializerFactory( const std::string & splitMethod );

    virtual std::unique_ptr<IonsInitializer> createIonsInitializer() const override;
    virtual std::unique_ptr<ElectromagInitializer> createElectromagInitializer() const override;
    virtual std::unique_ptr<OhmInitializer> createOhmInitializer() const override;
    virtual std::unique_ptr<SolverInitializer> createSolverInitializer() const override;
    virtual std::unique_ptr<BoundaryCondition> createBoundaryCondition() const override;

    virtual Box getBox() const override { return layout_.getBox() ; }
    virtual GridLayout const& gridLayout() const override { return layout_; }
    virtual double timeStep() const override { return dt_; }
    virtual std::string const & pusher() const override { return pusher_; }

    virtual std::vector<uint32> const &
    interpolationOrders() const override { return interpolationOrders_; }
    virtual std::vector<std::string> const &
    splittingStrategies() const override { return splitMethods_; }

    // virtual std::unique_ptr<DiagnosticInitializer> createDiagnosticInitializer() = 0;
    // virtual std::unique_ptr<GridLayoutInitializer> createGridLayoutInitializer() = 0;

private:

    GridLayout layout_;
    double dt_;

    const std::vector<uint32>  interpolationOrders_ ;
    const std::string pusher_ ;
    const std::vector<std::string> splitMethods_ ;

    void readInputFile();


};



#endif // SIMPLEINITIALIZERFACTORY_H


