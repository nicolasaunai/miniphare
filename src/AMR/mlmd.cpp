
#include "mlmd.h"
#include "patch.h"
#include "patchdata.h"
#include "Solver/solver.h"


MLMD::MLMD(std::unique_ptr<InitializerFactory> initFactory)
    : patchHierarchy_{ Patch { PatchData{ std::move(initFactory) }  } }
{
    // will probably have to change the way objects are initialized.
    // if we want, at some point, start from an already existing hierarchy
    // (in case of restart for e.g.
}



void MLMD::initializeRootLevel()
{
    Patch& rootLevel = patchHierarchy_.root();
    std::cout << "root level OK" << std::endl;
    rootLevel.init();

}
