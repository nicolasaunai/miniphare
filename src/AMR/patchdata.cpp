


#include "patchdata.h"
#include "hybridenums.h"

// contains all hybrid patch stuff (ions, fields, solver, etc.)

PatchData::PatchData(std::unique_ptr<InitializerFactory> initFactory)

    : EMfields_ { { {initFactory->gridLayout().allocSize(HybridQuantity::Ex ),
                     initFactory->gridLayout().allocSize(HybridQuantity::Ey ),
                     initFactory->gridLayout().allocSize(HybridQuantity::Ez )  }} ,

                  { {initFactory->gridLayout().allocSize(HybridQuantity::Bx ),
                     initFactory->gridLayout().allocSize(HybridQuantity::By ),
                     initFactory->gridLayout().allocSize(HybridQuantity::Bz )  }},
                  "_currentEMfield"
                },
      solver_{ initFactory->gridLayout(), initFactory->timeStep(),
               initFactory->createSolverInitializer() },

      ions_{ initFactory->gridLayout(), initFactory->createIonsInitializer() },
      boundaryCondition_{initFactory->createBoundaryCondition()}
{

}


void PatchData::init()
{
    std::cout << "init patch data" << std::endl;

    ions_.loadParticles();
    //EMfields_.init();
}


void PatchData::solveStep()
{
    solver_.solveStep(EMfields_, ions_, electrons_, boundaryCondition_.get());
}


