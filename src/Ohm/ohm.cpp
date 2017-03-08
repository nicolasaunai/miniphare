
#include "hybridenums.h"

#include "ohm.h"
#include "ohmimplfactory.h"




OhmImpl::OhmImpl(GridLayout const& layout, double eta, double nu)

    : idealTerm_ { layout.allocSize(HybridQuantity::Ex),
                   layout.allocSize(HybridQuantity::Ey),
                   layout.allocSize(HybridQuantity::Ez),
            { {HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez} },
            "_VexB" },

      pressureTerm_ { layout.allocSize(HybridQuantity::Ex),
                             layout.allocSize(HybridQuantity::Ey),
                             layout.allocSize(HybridQuantity::Ez),
                      { {HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez} },
                      "_gradPe" },
      resistivityTerm_ { layout.allocSize(HybridQuantity::Ex),
                             layout.allocSize(HybridQuantity::Ey),
                             layout.allocSize(HybridQuantity::Ez),
                      { {HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez} },
                      "_etaJ" },
      hyperResistivityTerm_ { layout.allocSize(HybridQuantity::Ex),
                             layout.allocSize(HybridQuantity::Ey),
                             layout.allocSize(HybridQuantity::Ez),
                      { {HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez} },
                      "_nuLapJ" },
      layout_{layout}, eta_{eta}, nu_{nu}
{}





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




