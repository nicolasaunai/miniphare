
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


    Field& Ex = Enew.component(VecField::VecX);
    Field& Ey = Enew.component(VecField::VecY);
    Field& Ez = Enew.component(VecField::VecZ);

    Field const& VexB_x = implPtr_->idealTerm_.component(VecField::VecX);
    Field const& VexB_y = implPtr_->idealTerm_.component(VecField::VecY);
    Field const& VexB_z = implPtr_->idealTerm_.component(VecField::VecZ);

    Field const& ePressureX = implPtr_->pressureTerm_.component(VecField::VecX);

    Field const& Rx     = implPtr_->resistivityTerm_.component(VecField::VecX);
    Field const& Ry     = implPtr_->resistivityTerm_.component(VecField::VecY);
    Field const& Rz     = implPtr_->resistivityTerm_.component(VecField::VecZ);


    for (uint32 i=0; i < Ex.size(); ++i)
    {
        Ex(i) = VexB_x(i) + 0*ePressureX(i) +0*Rx(i);
    }

    for (uint32 i=0; i < Ey.size(); ++i)
    {
        Ey(i) = VexB_y(i) +0* Ry(i);
    }

    for (uint32 i=0; i < Ez.size(); ++i)
    {
        Ez(i) = VexB_z(i) + 0*Rz(i);
    }
    std::cout << Ex(1) <<" " << Ex(2) <<" " <<Ex(3) <<" " <<Ex(4)<< std::endl;
}




