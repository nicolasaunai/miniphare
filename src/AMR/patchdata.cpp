


#include "patchdata.h"
#include "hybridenums.h"

// contains all hybrid patch stuff (ions, fields, solver, etc.)

PatchData::PatchData(const InitializerFactory* const  initFactory)

    : EMfields_ {initFactory->createElectromagInitializer()},
      solver_{ initFactory->gridLayout(), initFactory->timeStep(),
               initFactory->createSolverInitializer() },
      ions_{ initFactory->gridLayout(), initFactory->createIonsInitializer() },
      electrons_{initFactory->gridLayout(), 0.2},
      boundaryCondition_{initFactory->createBoundaryCondition()}//,
      //EMInitializer_{initFactory->createElectromagInitializer()}
{

}


void PatchData::init()
{
    std::cout << "Initializing PatchData..." << std::endl;
    ions_.loadParticles();
    solver_.init(ions_, boundaryCondition_.get() );
    //EMfields_.init(EMInitializer_.get());
    std::cout << "PatchData initialized!" << std::endl;
}


void PatchData::solveStep()
{
    solver_.solveStep(EMfields_, ions_, electrons_, boundaryCondition_.get());
}


