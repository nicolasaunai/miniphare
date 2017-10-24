
#include <utility>

#include "data/vecfield/vecfield.h"
#include "electromag.h"




Electromag::Electromag()
    : E_(AllocSizeT{}, AllocSizeT{}, AllocSizeT{},
         {{HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez}}, "Enew")
    , B_(AllocSizeT{}, AllocSizeT{}, AllocSizeT{},
         {{HybridQuantity::Bx, HybridQuantity::By, HybridQuantity::Bz}}, "Bnew")
{
}


Electromag::Electromag(std::array<AllocSizeT, NBR_COMPO> E_AllocSizes,
                       std::array<AllocSizeT, NBR_COMPO> B_AllocSizes, std::string name)
    : E_(E_AllocSizes[0], E_AllocSizes[1], E_AllocSizes[2],
         {{HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez}}, "E" + name)
    , B_(B_AllocSizes[0], B_AllocSizes[1], B_AllocSizes[2],
         {{HybridQuantity::Bx, HybridQuantity::By, HybridQuantity::Bz}}, "B" + name)
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



void Electromag::setFields(ElectromagInitializer& initializer)
{
    std::swap(E_, initializer.E_);
    std::swap(B_, initializer.B_);
}
