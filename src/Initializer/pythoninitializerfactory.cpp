#include "pythoninitializerfactory.h"


PythonInitializerFactory::PythonInitializerFactory()
    : layout_{ {0.1,0.,0.}, {42, 0, 0}, 1, "yee", 2 }, // hard-coded... will come from input somehow
      dt_{0.1}
{

}



std::unique_ptr<IonsInitializer> PythonInitializerFactory::createIonsInitializer() const
{
    return nullptr;
}


std::unique_ptr<ElectromagInitializer> PythonInitializerFactory::createElectromagInitializer() const
{
    return nullptr;
}


std::unique_ptr<OhmInitializer> PythonInitializerFactory::createOhmInitializer() const
{
    return nullptr;
}


std::string PythonInitializerFactory::pusherType() const
{
    std::string pusher = "modifiedBoris" ;

    return pusher ;
}

GridLayout const& PythonInitializerFactory::gridLayout() const
{
    return layout_;
}

double PythonInitializerFactory::timeStep() const
{
    return dt_;
}
