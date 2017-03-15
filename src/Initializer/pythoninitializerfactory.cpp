#include "pythoninitializerfactory.h"


PythonInitializerFactory::PythonInitializerFactory()
    : layout_{ {{0.1,0.,0.}}, {{42, 0, 0}}, 1, "yee", Point{0.,0.,0.}, 2 }, // hard-coded... will come from input somehow
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

std::unique_ptr<SolverInitializer> PythonInitializerFactory::createSolverInitializer() const
{
    return nullptr;
}

GridLayout const& PythonInitializerFactory::gridLayout() const
{
    return layout_;
}

double PythonInitializerFactory::timeStep() const
{
    return dt_;
}
