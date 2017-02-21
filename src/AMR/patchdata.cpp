


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
      boundaryCondition_{initFactory->createBoundaryCondition()},
      EMInitializer_{initFactory->createElectromagInitializer()}
{

}


void PatchData::init()
{
    std::cout << "Initializing PatchData..." << std::endl;
    ions_.loadParticles();
    EMfields_.init(EMInitializer_.get());
    std::cout << "PatchData initialized!" << std::endl;
}
