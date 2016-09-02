
#include<cmath>

#include "gridlayout.h"
#include "gridlayoutimplfactory.h"
#include "constants.h"



GridLayout::GridLayout(std::array<double,3> dxdydz, std::array<uint32,3> nbrCells,
                       uint32 nbDims      , std::string layoutName,
                       uint32 interpOrder )

    : dx_{dxdydz[0]}, dy_{dxdydz[1]}, dz_{dxdydz[2]},
      odx_{1./dx_}, ody_{1./dy_}, odz_{1./dz_},
      nbrCellx_{nbrCells[0]}, nbrCelly_{nbrCells[1]}, nbrCellz_{nbrCells[2]},
      interpOrder_{interpOrder},
      implPtr_{ GridLayoutImplFactory::createGridLayoutImpl(
                    nbDims, interpOrder, layoutName, nbrCells ) }
{
    switch (nbDims)
    {
        case 1:
            // 1D with tiny dx must be a problem
            if ( (std::abs(dx_) < EPS12) )
                throw std::runtime_error("Error - 1D requires non-zero dx");

            // cant' be 1D and have only 1 point in x
            if ( nbrCellx_ <= minNbrCells)
                throw std::runtime_error("Error - direction X is too small");


            // 1D but non-zero dy or dz
            if ( (std::abs(dy_) >  EPS12) || (std::abs(dz_) > EPS12) )
                throw std::runtime_error("Error - 1D requires dy=dz=0");

            // 1D but non-zero dimensions 2 and 3.
            if ( (nbrCelly_ != 0) || (nbrCellz_ != 0) )
                throw  std::runtime_error("Error - 1D requires ny=nz=1");

            break;

        case 2:
            if (std::abs(dz_) > EPS12)
              throw  std::runtime_error("Error - 2D requires dz = 0");

            if (nbrCellz_ != 0)
                throw  std::runtime_error("Error - 2D requires nz = 1");
        break;
    }

}



GridLayout::GridLayout(GridLayout const& source)
    : dx_{source.dx_}, dy_{source.dy_}, dz_{source.dz_},
      odx_{source.odx_},
      ody_{source.ody_},
      odz_{source.odz_},
      nbrCellx_{source.nbrCellx_},
      nbrCelly_{source.nbrCelly_},
      nbrCellz_{source.nbrCellz_},
      interpOrder_{source.interpOrder_}
{
    uint32 nbDims = source.nbDimensions();

    //TODO : "yee" bad hardcoded. make a clone
    implPtr_ =  GridLayoutImplFactory::createGridLayoutImpl(
                nbDims, interpOrder_, "yee", { {nbrCellx_, nbrCelly_, nbrCellz_} }) ;
}



GridLayout::GridLayout(GridLayout&& source)
    :  dx_{ std::move(source.dx_) },  dy_{std::move(source.dy_) }, dz_{std::move(source.dz_) },
      odx_{ std::move(source.odx_)}, ody_{std::move(source.ody_)}, odz_{std::move(source.odz_)},
      nbrCellx_{std::move(source.nbrCellx_)},
      nbrCelly_{std::move(source.nbrCelly_)},
      nbrCellz_{std::move(source.nbrCellz_)},
      implPtr_{std::move(source.implPtr_)}
{
}


uint32 GridLayout::nx() const
{
    return implPtr_->nx(nbrCellx_);
}



uint32 GridLayout::ny() const
{
    return implPtr_->ny(nbrCelly_);
}



uint32 GridLayout::nz() const
{
    return implPtr_->nz(nbrCellz_);
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

