


#include "patchdata.h"


// contains all hybrid patch stuff (ions, fields, solver, etc.)

PatchData::PatchData(std::unique_ptr<InitializerFactory> initFactory)

    : EMfields_{ initFactory->gridLayout().nx(),
                 initFactory->gridLayout().ny(),
                 initFactory->gridLayout().nz(), "_currentEMfield"},
      solver_{initFactory->gridLayout(), initFactory->timeStep() },
      ions_{initFactory->gridLayout(), *initFactory->createIonsInitializer() }

{

}

