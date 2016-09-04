
#include<cmath>

#include "gridlayout.h"
#include "gridlayoutimplfactory.h"
#include "constants.h"
#include "utility.h"




/* ---------------------------------------------------------------------------
 *
 *                                  PUBLIC
 *
 * --------------------------------------------------------------------------- */




GridLayout::GridLayout(std::array<double,3> dxdydz, std::array<uint32,3> nbrCells,
                       uint32 nbDims, std::string layoutName)

    : dx_{dxdydz[0]}, dy_{dxdydz[1]}, dz_{dxdydz[2]},
      odx_{1./dx_}, ody_{1./dy_}, odz_{1./dz_},
      nbrCellx_{nbrCells[0]}, nbrCelly_{nbrCells[1]}, nbrCellz_{nbrCells[2]},
      implPtr_{ GridLayoutImplFactory::createGridLayoutImpl(nbDims, layoutName) }
{

    switch (nbDims)
    {
    case 1:
        throwNotValid1D();
        break;

    case 2:
        throwNotValid2D();
        break;

    case 3:
        throwNotValid3D();
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
      nbrCellz_{source.nbrCellz_}
{
    uint32 nbDims = source.nbDimensions();
    implPtr_ =  GridLayoutImplFactory::createGridLayoutImpl(nbDims, "yee") ; //TODO bad hardcoded. make a clone
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


/* ---------------------------------------------------------------------------
 *
 *                                  PRIVATE
 *
 * --------------------------------------------------------------------------- */


const std::string GridLayout::errorInverseMesh= "GridLayout error: Invalid use of";




void GridLayout::throwNotValid1D()const
{
    // 1D with tiny dx must be a problem
    if (dx_ == 0.0)
        throw std::runtime_error("Error - 1D requires non-zero dx");

    // dx should be > 0
    if ( dx_ < 0)
        throw std::runtime_error("Error - 1D requires positive dx");

    // cant' be 1D and have less than minNbrCells point in X
    if ( nbrCellx_ < minNbrCells)
        throw std::runtime_error("Error - direction X is too small");

    // 1D but non-zero dy or dz
    if (dy_ != 0 || dz_ )
        throw std::runtime_error("Error - 1D requires dy=dz=0");

    // 1D but non-zero dimensions 2 and 3.
    if ( (nbrCelly_ != 0) || (nbrCellz_ != 0) )
        throw  std::runtime_error("Error - 1D requires ny=nz=1");

}




void GridLayout::throwNotValid2D() const
{
    if ( dx_ == 0. || dy_ == 0. )
        throw std::runtime_error("Error - 2D requires both dx and dy to be non-zero");

    // dx and dy should be > 0
    if ( dx_ < 0. || dy_ < 0.)
        throw std::runtime_error("Error - 2D requires positive dx and dy");



    if ( dz_ != 0.)
        throw  std::runtime_error("Error - 2D requires dz = 0");


    // cant' be 2D and have less than minNbrCells point in X
    if ( nbrCellx_ < minNbrCells || nbrCelly_ < minNbrCells)
        throw std::runtime_error("Error - too few Cells in non-invariant directions");
}





void GridLayout::throwNotValid3D() const
{
    if (   (std::abs(dx_) < EPS12)
        || (std::abs(dy_) < EPS12)
        || (std::abs(dz_) < EPS12) )
        throw std::runtime_error("Error - 3D requires dx, dy, dz to be all non-zero");

    // dx dy and dz should be > 0
    if ( dx_ < 0. || dy_ < 0.|| dz_ < 0.)
        throw std::runtime_error("Error - 2D requires positive dx and dy");


    // cant' be 2D and have less than minNbrCells point in X
    if (    nbrCellx_ < minNbrCells
         || nbrCelly_ < minNbrCells
         || nbrCellz_ < minNbrCells)
        throw std::runtime_error("Error - too few Cells in non-invariant directions");
}

