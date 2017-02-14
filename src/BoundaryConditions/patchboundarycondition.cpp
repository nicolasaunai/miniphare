#include "patchboundarycondition.h"


PatchBoundaryCondition::PatchBoundaryCondition(std::unique_ptr<InitializerFactory> initFactory)
    : EMfields_ { { {initFactory->gridLayout().allocSize(HybridQuantity::Ex ),
                     initFactory->gridLayout().allocSize(HybridQuantity::Ey ),
                     initFactory->gridLayout().allocSize(HybridQuantity::Ez )  }} ,

                  { {initFactory->gridLayout().allocSize(HybridQuantity::Bx ),
                     initFactory->gridLayout().allocSize(HybridQuantity::By ),
                     initFactory->gridLayout().allocSize(HybridQuantity::Bz )  }},
                  "_PRAEMFields"
                },
      ions_{ initFactory->gridLayout(), initFactory->createIonsInitializer() }
{

}
