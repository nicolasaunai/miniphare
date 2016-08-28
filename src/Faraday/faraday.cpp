
#include "faraday.h"
#include "faradayfactory.h"
#include "grid/gridlayout.h"


Faraday::Faraday(double dt, GridLayout const& layout)
    : implPtr_{FaradayImplFactory::createFaradayImpl(dt, layout)}
{
}




void Faraday::operator()(VecField const& E, VecField const& B, VecField& Bnew)
{
    return (*implPtr_)(E, B, Bnew);
}


FaradayImpl::~FaradayImpl(){} // why here














