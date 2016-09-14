#ifndef PYTHONINITIALIZERFACTORY_H
#define PYTHONINITIALIZERFACTORY_H


#include "initializerfactory.h"



class PythonInitializerFactory : public InitializerFactory
{
public:

    PythonInitializerFactory()                                                 = default;
    PythonInitializerFactory(PythonInitializerFactory const&source)            = default;
    PythonInitializerFactory(PythonInitializerFactory&& source)                = default;
    PythonInitializerFactory& operator=(PythonInitializerFactory const&source) = default;
    PythonInitializerFactory& operator=(PythonInitializerFactory&& source)     = default;



    virtual std::unique_ptr<IonsInitializer> createIonsInitializer() const override;
    virtual std::unique_ptr<ElectromagInitializer> createElectromagInitializer() const override;
    virtual std::unique_ptr<OhmInitializer> createOhmInitializer() const override;


};

#endif // PYTHONINITIALIZERFACTORY_H
