
#include <utility>

#include "constants.h"
#include "electromag.h"
#include "vecfield/vecfield.h"


Electromag::Electromag( std::array<AllocSizeT, NBR_COMPO> E_AllocSizes,
                        std::array<AllocSizeT, NBR_COMPO> B_AllocSizes,
                        std::string name )
    : E_(  E_AllocSizes[0], E_AllocSizes[1], E_AllocSizes[2],
           { {HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez} },
           "E" + name ),


      B_(  B_AllocSizes[0], B_AllocSizes[1], B_AllocSizes[2],
            { {HybridQuantity::Bx, HybridQuantity::By, HybridQuantity::Bz} },
            "B" + name)
{

}



Electromag::Electromag(std::unique_ptr<ElectromagInitializer> initializer)
{
    // we swap the VecFields with those of the initializer
    // the original initializer has been MOVED to this function
    // and that one dies at the end of the function so the swapped
    // data is deallocated.
    std::swap(E_, initializer->E_);
    std::swap(B_, initializer->B_);
}
