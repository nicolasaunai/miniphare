

#include "ohm.h"
#include "ohmimplfactory.h"



OhmImplInternals::OhmImplInternals(GridLayout const& layout)
    : ideal_{ layout.nx(), layout.ny(), layout.nz(), "_ideal" },
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




