
#include<algorithm>


#include "ohm.h"
#include "hybridenums.h"
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

     implPtr_->computeTerms(B, Ne, Ve, Pe, J);


    Field& Ex = Enew.component(0);
    Field& Ey = Enew.component(1);
    Field& Ez = Enew.component(2);

    Field const& VexB_x = implPtr_->idealTerm_.component(0);
    Field const& VexB_y = implPtr_->idealTerm_.component(1);
    Field const& VexB_z = implPtr_->idealTerm_.component(2);

    Field const& ePressureX = implPtr_->pressureTerm_.component(0);
    Field const& ePressureY = implPtr_->pressureTerm_.component(1);
    Field const& ePressureZ = implPtr_->pressureTerm_.component(2);

    Field const& Rx     = implPtr_->resistivityTerm_.component(0);
    Field const& Ry     = implPtr_->resistivityTerm_.component(1);
    Field const& Rz     = implPtr_->resistivityTerm_.component(2);


    for (uint32 i=0; i < Ex.size(); ++i)
    {
        Ex(i) = VexB_x(i) + ePressureX(i) + Rx(i);
    }

    for (uint32 i=0; i < Ey.size(); ++i)
    {
        Ey(i) = VexB_y(i) + ePressureY(i) + Ry(i);
    }

    for (uint32 i=0; i < Ez.size(); ++i)
    {
        Ez(i) = VexB_z(i) + ePressureZ(i) + Rz(i);
    }
}




