#ifndef PYTHONINITIALIZERFACTORY_H
#define PYTHONINITIALIZERFACTORY_H


#include "simulationinitializerfactory.h"
#include "grid/gridlayout.h"


/**
 * @brief The PythonInitializerFactory class is a concrete InitializerFactory
 * used to create objects from a user supplied python script. See InitializerFactory
 * for documentation of its methods.
 */
class PythonInitializerFactory : public SimulationInitializerFactory
{
public:

    PythonInitializerFactory();
    PythonInitializerFactory(PythonInitializerFactory const&source)            = default;
    PythonInitializerFactory(PythonInitializerFactory&& source)                = default;
    PythonInitializerFactory& operator=(PythonInitializerFactory const&source) = default;
    PythonInitializerFactory& operator=(PythonInitializerFactory&& source)     = default;


    virtual std::unique_ptr<IonsInitializer> createIonsInitializer()             const override;
    virtual std::unique_ptr<ElectromagInitializer> createElectromagInitializer() const override;

    virtual std::unique_ptr<SolverInitializer> createSolverInitializer()         const override;

    virtual GridLayout const& gridLayout() const override;
    virtual double timeStep() const override;



private:
    GridLayout layout_;
    double dt_;
};

#endif // PYTHONINITIALIZERFACTORY_H
