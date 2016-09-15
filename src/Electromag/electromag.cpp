
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



