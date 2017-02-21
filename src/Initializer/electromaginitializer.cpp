#include "electromaginitializer.h"


ElectromagInitializer::ElectromagInitializer(GridLayout const& layout,
                                             VectorFunction electricField,
                                             VectorFunction magneticField)
    : electricField{electricField},
      magneticField{magneticField},
      E_    { layout.allocSize(HybridQuantity::Ex),
              layout.allocSize(HybridQuantity::Ey),
              layout.allocSize(HybridQuantity::Ez),
              { {HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez} },
              "_electricField" },
      B_    { layout.allocSize(HybridQuantity::Bx),
              layout.allocSize(HybridQuantity::By),
              layout.allocSize(HybridQuantity::Bz),
              { {HybridQuantity::Bx, HybridQuantity::By, HybridQuantity::Bz} },
              "_magneticField" }

{

}
