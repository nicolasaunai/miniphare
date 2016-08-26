#include "gridlayout.h"
#include "gridlayoutimplfactory.h"


  //GridLayoutImpl::~GridLayoutImpl(){}


GridLayoutImplInternals::GridLayoutImplInternals(uint32 nbDims)
     : nbdims_{nbDims}
{
}




GridLayout::GridLayout()
    : dx_{0.1}, dy_{1}, dz_{1},
      odx_{1./dx_}, ody_{1./dy_}, odz_{1./dz_},
      nx_{10}, ny_{1}, nz_{1},
      implPtr_{GridLayoutImplFactory::createGridLayoutImpl(1)}
{
}



GridLayout::GridLayout(GridLayout const& source)
    :dx_{source.dx_}, dy_{source.dy_}, dz_{source.dz_},
      odx_{source.odx_}, ody_{source.ody_}, odz_{source.odz_},
      nx_{source.nx_}, ny_{source.ny_}, nz_{source.nz_}
{
    uint32 nbDims = source.nbDimensions();
    implPtr_ =  GridLayoutImplFactory::createGridLayoutImpl(nbDims) ;
}







uint32 GridLayout::physicalStartIndex(Field const& field, uint32 direction) const
{
    return implPtr_->physicalStartIndex(field, direction);
}


uint32 GridLayout::physicalEndIndex(Field const& field, uint32 direction) const
{
    return  implPtr_->physicalEndIndex(field, direction);
}



uint32 GridLayout::ghostStartIndex(Field const& field, uint32 direction) const
{
    return implPtr_->ghostStartIndex(field, direction);
}



uint32 GridLayout::ghostEndIndex  (Field const& field, uint32 direction) const
{
    return implPtr_->ghostStartIndex(field, direction);
}



void GridLayout::deriv(Field const& operand, uint32 direction, Field& derivative)const
{
    implPtr_->deriv(operand, direction, derivative);
}




uint32 GridLayout::nbDimensions() const
{
    return implPtr_->nbDimensions();
}

