
#include<cmath>

#include "gridlayout.h"
#include "gridlayoutimplfactory.h"
#include "constants.h"

  //GridLayoutImpl::~GridLayoutImpl(){}


GridLayoutImplInternals::GridLayoutImplInternals(uint32 nbDims)
     : nbdims_{nbDims}
{
}




GridLayout::GridLayout(std::array<double,3> dxdydz, std::array<uint32,3> fieldSizes,
                       uint32 nbDims, std::string layoutName)
    : dx_{dxdydz[0]}, dy_{dxdydz[1]}, dz_{dxdydz[2]},
      odx_{1./dx_}, ody_{1./dy_}, odz_{1./dz_},
      nx_{fieldSizes[0]}, ny_{fieldSizes[1]}, nz_{fieldSizes[2]},
      implPtr_{GridLayoutImplFactory::createGridLayoutImpl(nbDims, layoutName)}
{
    switch (nbDims)
    {
        case 1:
            // 1D with tiny dx must be a problem
            if ( (std::abs(dx_) < EPS12) )
                throw std::runtime_error("Error - 1D requires non-zero dx");

            // cant' be 1D and have only 1 point in x
            //if ()


            // 1D but non-zero dy or dz
            if ( (std::abs(dy_) >  EPS12) || (std::abs(dz_) > EPS12) )
                throw std::runtime_error("Error - 1D requires dy=dz=0");

            // 1D but non-zero dimensions 2 and 3.
            if ( (ny_ != 1) || (nz_ != 1) )
                throw  std::runtime_error("Error - 1D requires ny=nz=1");

        case 2:
            if (std::abs(dz_) > EPS12)
              throw  std::runtime_error("Error - 2D requires dz = 0");

            if (nz_ != 1)
                throw  std::runtime_error("Error - 2D requires nz = 1");
    }
}



GridLayout::GridLayout(GridLayout const& source)
    :dx_{source.dx_}, dy_{source.dy_}, dz_{source.dz_},
      odx_{source.odx_}, ody_{source.ody_}, odz_{source.odz_},
      nx_{source.nx_}, ny_{source.ny_}, nz_{source.nz_}
{
    uint32 nbDims = source.nbDimensions();
    implPtr_ =  GridLayoutImplFactory::createGridLayoutImpl(nbDims, "yee") ;
}



GridLayout::GridLayout(GridLayout&& source)
    : dx_{ std::move(source.dx_) }, dy_{ std::move(source.dy_) },dz_{ std::move(source.dz_) },
      odx_{ std::move(source.odx_)}, ody_{std::move(source.ody_)}, odz_{std::move(source.odz_)},
      nx_{std::move(source.nx_)}, ny_{std::move(source.ny_)}, nz_{std::move(source.nz_)},
      implPtr_{std::move(source.implPtr_)}
{
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

