#ifndef SIMPLEINITIALIZERFACTORY_H
#define SIMPLEINITIALIZERFACTORY_H


#include "simulationinitializerfactory.h"
#include "utilities/Time/pharetime.h"



/**
 * @brief The SimpleInitializerFactory class is a concrete InitializerFactory
 * used to simply initialize miniphare with hard-coded parameters. It is mainly
 * used for development purposes. The SumpleInitializerFactory will create
 * ions etc. from the FluidParticleInitializer
 */
class SimpleInitializerFactory : public SimulationInitializerFactory
{
public:
    // TODO : see what Ctors will need to be defined
    SimpleInitializerFactory();
    SimpleInitializerFactory(const std::string& splitMethod);

    virtual std::unique_ptr<IonsInitializer> createIonsInitializer() const override;
    virtual std::unique_ptr<ElectromagInitializer> createElectromagInitializer() const override;
    virtual std::unique_ptr<SolverInitializer> createSolverInitializer() const override;
    virtual std::unique_ptr<BoundaryCondition> createBoundaryCondition() const override;
    virtual std::unique_ptr<DiagnosticInitializer> createDiagnosticInitializer() const override;
    virtual std::unique_ptr<MLMDInitializer> createMLMDInitializer() const override;
    virtual std::unique_ptr<Time> createTimeManager() const override;

    virtual Box getBox() const override { return layout_.getBox(); }
    virtual GridLayout const& gridLayout() const override { return layout_; }
    virtual double timeStep() const override { return createTimeManager()->timeStep(); }
    virtual std::string const& pusher() const override { return pusher_; }

    virtual bool inputError() const override { return false; }

    virtual uint32 interpolationOrder() const override { return interpolationOrder_; }
    virtual std::vector<std::string> const& splittingStrategies() const override
    {
        return splitMethods_;
    }

    // virtual std::unique_ptr<GridLayoutInitializer> createGridLayoutInitializer() = 0;

private:
    Time timeManager_;
    GridLayout layout_;

    const uint32 interpolationOrder_;
    const std::string pusher_;
    const std::vector<std::string> splitMethods_;
};



#endif // SIMPLEINITIALIZERFACTORY_H
