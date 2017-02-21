
#include "mlmd.h"
#include "hierarchy.h"
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


void MLMD::evolveFullDomain()
{

    // evolve fields and particle for a time step
    patchHierarchy_.evolveHierarchy() ;

    // Here, AMR patches will say whether they need refinement
    // the ouput of this method is used by updateHierarchy()
    patchHierarchy_.evaluateHierarchy() ;

    // new patches are created here if necessary
    // it depends on evaluateHierarchy()
    patchHierarchy_.updateHierarchy() ;


}


