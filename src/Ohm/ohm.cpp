
#include "gridconstants.h"

#include "ohm.h"
#include "ohmimplfactory.h"



OhmImplInternals::OhmImplInternals(GridLayout const& layout)
    : idealTerm_{ layout.allocSize(OhmTerm::Ideal),
      {{HybridQuantity::Ohm, HybridQuantity::Ohm, HybridQuantity::Ohm}}, "_ideal" },
      pressureTerm_{ layout.allocSize(OhmTerm::Pressure),
      {{HybridQuantity::Ohm, HybridQuantity::Ohm, HybridQuantity::Ohm}}, "_pressure" },
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
                      VecField const& Ve, Field const& Pe, VecField& Enew)
{
    (*implPtr_)(B, Ne, Ve, Pe, Enew);
}




