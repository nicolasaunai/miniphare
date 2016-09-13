
#include "constants.h"

#include "electromag.h"
#include "vecfield/vecfield.h"


Electromag::Electromag( std::array<AllocSizeT, NBR_COMPO> E_AllocSizes,
                        std::array<AllocSizeT, NBR_COMPO> B_AllocSizes,
                        std::string name )
    : E_( E_AllocSizes, {{HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez}},
          "E"+name ),
      B_( B_AllocSizes, {{HybridQuantity::Bx, HybridQuantity::By, HybridQuantity::Bz}},
          "B"+name )
{

}



