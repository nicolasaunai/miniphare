#ifndef PYTHONINITIALIZERFACTORY_H
#define PYTHONINITIALIZERFACTORY_H


#include "initializerfactory.h"
#include "grid/gridlayout.h"



class PythonInitializerFactory : public InitializerFactory
{
public:

    PythonInitializerFactory();
    PythonInitializerFactory(PythonInitializerFactory const&source)            = default;
    PythonInitializerFactory(PythonInitializerFactory&& source)                = default;
    PythonInitializerFactory& operator=(PythonInitializerFactory const&source) = default;
    PythonInitializerFactory& operator=(PythonInitializerFactory&& source)     = default;


    virtual std::unique_ptr<IonsInitializer> createIonsInitializer()             const override;
    virtual std::unique_ptr<ElectromagInitializer> createElectromagInitializer() const override;
    virtual std::unique_ptr<OhmInitializer> createOhmInitializer()               const override;


    virtual GridLayout const& gridLayout() const override;
    virtual double timeStep() const override;



private:
    GridLayout layout_;
    double dt_;
};

#endif // PYTHONINITIALIZERFACTORY_H
