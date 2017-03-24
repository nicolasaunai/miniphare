
#include <algorithm>
#include <functional>

#include "electrons.h"
#include "electronsimplfactory.h"


Electrons::Electrons(GridLayout const& layout, double Te)
    :  Impl_{ElectronsImplFactory::createElectronImpl(layout,Te)}
{

}







