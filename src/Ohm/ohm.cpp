
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
    //(*implPtr_)(B, Ne, Ve, Pe, J, Enew);

    Field& Ex = Enew.component(0);
    Field& Ey = Enew.component(1);
    Field& Ez = Enew.component(2);

    Field const& VexB_x = implPtr_->idealTerm_.component(0);
    Field const& VexB_y = implPtr_->idealTerm_.component(1);
    Field const& VexB_z = implPtr_->idealTerm_.component(2);

    Field const& gradPx = implPtr_->pressureTerm_.component(0);
    Field const& gradPy = implPtr_->pressureTerm_.component(1);
    Field const& gradPz = implPtr_->pressureTerm_.component(2);

    Field const& Rx     = implPtr_->resistivityTerm_.component(0);
    Field const& Ry     = implPtr_->resistivityTerm_.component(1);
    Field const& Rz     = implPtr_->resistivityTerm_.component(2);


    // add the ideal term
    std::transform(Ex.begin(), Ex.end(), VexB_x.begin(), Ex.begin(), std::plus<double>());
    std::transform(Ey.begin(), Ey.end(), VexB_y.begin(), Ey.begin(), std::plus<double>());
    std::transform(Ez.begin(), Ez.end(), VexB_z.begin(), Ez.begin(), std::plus<double>());

    // add the pressure term
    std::transform(Ex.begin(), Ex.end(), gradPx.begin(), Ex.begin(), std::plus<double>());
    std::transform(Ey.begin(), Ey.end(), gradPy.begin(), Ey.begin(), std::plus<double>());
    std::transform(Ez.begin(), Ez.end(), gradPz.begin(), Ez.begin(), std::plus<double>());

    // add resistive term
    std::transform(Ex.begin(), Ex.end(), Rx.begin(), Ex.begin(), std::plus<double>());
    std::transform(Ey.begin(), Ey.end(), Ry.begin(), Ey.begin(), std::plus<double>());
    std::transform(Ez.begin(), Ez.end(), Rz.begin(), Ez.begin(), std::plus<double>());

}




