
#include "patchdata.h"
#include "utilities/hybridenums.h"
#include "utilities/print/outputs.h"



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
    ions_.loadParticles();
    solver_.init(ions_, *boundaryCondition_);
}



uint32 PatchData::population() const
{
    return ions_.population();
}



void PatchData::solveStep()
{
    solver_.solveStepPPC(EMfields_, ions_, electrons_, *boundaryCondition_);
}
