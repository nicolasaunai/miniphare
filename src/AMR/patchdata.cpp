


#include "patchdata.h"
#include "hybridenums.h"

// contains all hybrid patch stuff (ions, fields, solver, etc.)

PatchData::PatchData(std::unique_ptr<InitializerFactory> initFactory)

    : EMfields_ { { {initFactory->gridLayout().allocSize(HybridQuantity::Ex ),
                     initFactory->gridLayout().allocSize(HybridQuantity::Ey ),
                     initFactory->gridLayout().allocSize(HybridQuantity::Ez )  }} ,

                  { {initFactory->gridLayout().allocSize(HybridQuantity::Ex ),
                     initFactory->gridLayout().allocSize(HybridQuantity::Ey ),
                     initFactory->gridLayout().allocSize(HybridQuantity::Ez )  }},
                  "_currentEMfield"
                },
      solver_{ initFactory->gridLayout(), initFactory->timeStep() },

      ions_{ initFactory->gridLayout(), initFactory->createIonsInitializer() }
{

}


void PatchData::init()
{
        std::cout << "init patch data" << std::endl;
    ions_.loadParticles();
    //EMfields_.init();
}
