
#include "patchdata.h"
#include "utilities/hybridenums.h"




PatchData::PatchData(const InitializerFactory& initFactory)

    : EMfields_{initFactory.createElectromagInitializer()}
    , solver_{initFactory.gridLayout(), initFactory.timeStep(),
              initFactory.createSolverInitializer()}
    , ions_{initFactory.gridLayout(), initFactory.createIonsInitializer()}
    , electrons_{initFactory.gridLayout(), 0.2}
    , boundaryCondition_{initFactory.createBoundaryCondition()}
// EMInitializer_{initFactory->createElectromagInitializer()}
{
}


void PatchData::initPatchPhysicalDomain()
{
    std::cout << "Initializing PatchData..." << std::endl;

    ions_.loadParticles();

    solver_.init(ions_, *boundaryCondition_);
    // EMfields_.init(EMInitializer_.get());
    std::cout << "PatchData initialized!" << std::endl;
}



uint32 PatchData::population() const
{
    return ions_.population();
}



void PatchData::solveStep()
{
    solver_.solveStepPPC(EMfields_, ions_, electrons_, *boundaryCondition_);
}
