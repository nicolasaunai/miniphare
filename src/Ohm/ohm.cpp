
#include "hybridenums.h"

#include "ohm.h"
#include "ohmimplfactory.h"




OhmImpl::OhmImpl(GridLayout const& layout)
    : idealTerm_{   layout.allocSize(HybridQuantity::Ex),
                    layout.allocSize(HybridQuantity::Ey),
                    layout.allocSize(HybridQuantity::Ez),
                    { {HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez} },
                    "_ideal"},

      pressureTerm_{layout.allocSize(HybridQuantity::Ex),
                    layout.allocSize(HybridQuantity::Ey),
                    layout.allocSize(HybridQuantity::Ez),
                    { {HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez} },
                    "_pressure" },
      resistivityTerm_{layout.allocSize(HybridQuantity::Ex),
                       layout.allocSize(HybridQuantity::Ey),
                       layout.allocSize(HybridQuantity::Ez),
                    { {HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez} },
                    "_resistivity" },
      hyperResistivityTerm_{layout.allocSize(HybridQuantity::Ex),
                            layout.allocSize(HybridQuantity::Ey),
                            layout.allocSize(HybridQuantity::Ez),
                    { {HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez} },
                    "_hyperresistivity" },

      layout_{ layout }
{
}


Ohm::Ohm(GridLayout const& layout)
    :implPtr_ { OhmImplFactory::createOhmImpl(layout)  }
{
}


OhmImpl::~OhmImpl()
{
}


//ohm(Bpred, Ve, Pe, Epred);//
 void Ohm::operator()(VecField const& B,  Field const& Ne,
                      VecField const& Ve, Field const& Pe,
                      VecField const&J, VecField& Enew)
{
    (*implPtr_)(B, Ne, Ve, Pe, J, Enew);
}




