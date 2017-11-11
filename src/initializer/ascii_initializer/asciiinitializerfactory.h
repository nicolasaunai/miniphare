#ifndef ASCIIINITIALIZERFACTORY_H
#define ASCIIINITIALIZERFACTORY_H



#include "inidata.h"
#include "iniextractor.h"
#include "initializer/initmodel/initmodel.h"
#include "initializer/simulationinitializerfactory.h"
#include "initializer/solverinitializer.h"
#include "utilities/Time/pharetime.h"

#include <string>




/**
 * @brief The AsciiInitializerFactory class is a concrete InitializerFactory
 * used to  initialize miniphare from an ascii parameter file.
 */
class AsciiInitializerFactory : public SimulationInitializerFactory
{
public:
    // TODO : see what Ctors will need to be defined
    AsciiInitializerFactory() = delete;
    AsciiInitializerFactory(std::string const& filename);

    virtual std::unique_ptr<IonsInitializer> createIonsInitializer() const override;
    virtual std::unique_ptr<ElectromagInitializer> createElectromagInitializer() const override;
    virtual std::unique_ptr<SolverInitializer> createSolverInitializer() const override;
    virtual std::unique_ptr<BoundaryCondition> createBoundaryCondition() const override;
    virtual std::unique_ptr<DiagnosticInitializer> createDiagnosticInitializer() const override;
    virtual std::unique_ptr<Time> createTimeManager() const override;

    virtual bool inputError() const override { return iniData_.error(); }

    virtual Box getBox() const override { return layout_.getBox(); }
    virtual GridLayout const& gridLayout() const override { return layout_; }
    virtual double timeStep() const override { return createTimeManager()->timeStep(); }
    virtual std::string const& pusher() const override { return iniData_.pusherName; }

    virtual std::vector<uint32> const& interpolationOrders() const override
    {
        return interpOrders_;
    }
    virtual std::vector<std::string> const& splittingStrategies() const override
    {
        return splitMethods_;
    }

    // virtual std::unique_ptr<DiagnosticInitializer> createDiagnosticInitializer() = 0;
    // virtual std::unique_ptr<GridLayoutInitializer> createGridLayoutInitializer() = 0;

private:
    INIData iniData_;
    GridLayout layout_;
    std::unique_ptr<INIExtractor> iniExtractor_;
    std::unique_ptr<InitModel> initModel_;
    const std::vector<std::string> splitMethods_;
    std::vector<uint32> interpOrders_;

    /*
    Time timeManager_;

    const std::vector<uint32> interpolationOrders_;
    const std::string pusher_;
    const std::vector<std::string> splitMethods_;
    */
};


#endif // ASCIIINITIALIZERFACTORY_H
