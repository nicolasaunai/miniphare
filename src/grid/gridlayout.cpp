
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



/**
 * @brief GridLayout::GridLayout constructs a GridLayout
 * @param dxdydz  mesh resolution in each direction. Used dimensions must
 * have a non-zero mesh size and vice-versa otherwise a runtime_error exception
 * is thrown.
 * @param nbrCells number of cells in the physical domain in each direction.
 * All used dimensions must have at least GridLayout::minNbrCells=10 cells otherwise
 * a runtime_error exception is thrown. Each non-zero nbr Cell must be associated
 * with a non-zero mesh size (dxdydz) and vice-versa.
 * @param nbDims number of dimenions in the problem, can be 1, 2 or 3
 * @param layoutName is the name of the specific Grid Layout desired. For now only
 * 'yee' is available.
 * @param ghostParameter is an integer GridLayout uses to determine the number
 * of ghost nodes required for each quantity in each direction.
 */
GridLayout::GridLayout(std::array<double,3> dxdydz, std::array<uint32,3> nbrCells,
                       uint32 nbDims      , std::string layoutName, Point origin,
                       uint32 ghostParameter )

    : nbDims_{nbDims},
      dx_{dxdydz[0]}, dy_{dxdydz[1]}, dz_{dxdydz[2]},
      odx_{1./dx_}, ody_{1./dy_}, odz_{1./dz_},
      nbrCellx_{nbrCells[0]}, nbrCelly_{nbrCells[1]}, nbrCellz_{nbrCells[2]},
      origin_{origin},
      interpOrder_{ghostParameter},
      implPtr_{ GridLayoutImplFactory::createGridLayoutImpl(
                    nbDims, origin, ghostParameter, layoutName, nbrCells, dxdydz ) }
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
    : nbDims_{source.nbDims_},
      dx_{source.dx_}, dy_{source.dy_}, dz_{source.dz_},
      odx_{source.odx_},
      ody_{source.ody_},
      odz_{source.odz_},
      nbrCellx_{source.nbrCellx_},
      nbrCelly_{source.nbrCelly_},
      nbrCellz_{source.nbrCellz_},
      origin_{source.origin_},
      interpOrder_{source.interpOrder_}
{
    //TODO : "yee" bad hardcoded. make a clone
    implPtr_ =  GridLayoutImplFactory::createGridLayoutImpl(
                nbDims_, origin_, interpOrder_, "yee", { {nbrCellx_, nbrCelly_, nbrCellz_} },
                { {dx_, dy_, dz_} } ) ;
}



GridLayout::GridLayout(GridLayout&& source)
    :  dx_{ std::move(source.dx_) },  dy_{std::move(source.dy_) }, dz_{std::move(source.dz_) },
      odx_{ std::move(source.odx_)}, ody_{std::move(source.ody_)}, odz_{std::move(source.odz_)},
      nbrCellx_{std::move(source.nbrCellx_)},
      nbrCelly_{std::move(source.nbrCelly_)},
      nbrCellz_{std::move(source.nbrCellz_)},
      origin_{std::move(source.origin_)},
      interpOrder_{std::move(source.interpOrder_)},
      implPtr_{std::move(source.implPtr_)}
{
}





/**
 * @brief GridLayout::physicalStartIndex return the index of the first node
 * of a 'field' in a given 'direction' that belong to the physical domain.
 * The function just calls its private implementation
 */
uint32 GridLayout::physicalStartIndex(Field const& field, Direction direction) const
{
    return implPtr_->physicalStartIndex(field, direction);
}




/**
 * @brief GridLayout::physicalStartIndex return the index of the first primal or
 * dual node (centering= QtyCentering::dual or QtyCentering::primal).
 * The function just calls its private implementation.
 */
uint32 GridLayout::physicalStartIndex( QtyCentering centering, Direction direction ) const
{
    return implPtr_->physicalStartIndex( centering, direction ) ;
}




/**
 * @brief GridLayout::physicalEndIndex return the index of the last node of a
 * 'field' in a given 'direction'. The function just calls its private implementation
 */
uint32 GridLayout::physicalEndIndex(Field const& field, Direction direction) const
{
    return  implPtr_->physicalEndIndex(field, direction);
}





/**
 * @brief GridLayout::physicalEndIndex return the index of the last primal or
 * dual node in a given direction.
 * @param centering is either QtyCentering::dual or QtyCentering::primal
 */
uint32 GridLayout::physicalEndIndex( QtyCentering centering, Direction direction ) const
{
    return implPtr_->physicalEndIndex( centering, direction ) ;
}





/**
 * @brief GridLayout::ghostStartIndex returns the index of the first ghost node
 * of a 'field' in a given 'direction'. The function just calls its private
 * implementation.
 */
uint32 GridLayout::ghostStartIndex(Field const& field, Direction direction) const
{
    // TODO should this not always be 0?
    return implPtr_->ghostStartIndex(field, direction);
}




/**
 * @brief GridLayout::ghostEndIndex returns the index of the last primal or dual
 * node in a given 'direction'.
 */
uint32 GridLayout::ghostEndIndex  (Field const& field, Direction direction) const
{
    return implPtr_->ghostEndIndex(field, direction);
}




/**
 * @brief GridLayout::deriv calculate and return the derivative of a Field
 * in a given direction. In practice the function just calls its private implementation
 */
void GridLayout::deriv(Field const& operand, Direction direction, Field& derivative)const
{
    switch(nbDims_)
    {
    case 1:
        implPtr_->deriv1D(operand, derivative);
        break ;

    default:
        throw std::runtime_error("Error - only 1D fields are handled for the moment ");
    }

}




/**
 * @brief GridLayout::allocSize
 * @return An AllocSizeT object, containing the allocation size of arrays in all
 * directions
 */
AllocSizeT GridLayout::allocSize(HybridQuantity qtyType) const
{
    return implPtr_->allocSize( qtyType ) ;
}




/**
 * @brief GridLayout::allocSizeDerived calculate the allocation size in each
 * direction for a field that is the first derivative of an HybridQuantity in
 * a given direction.
 * @param qty the quantity to be derivated
 * @param dir direciton of derivation
 * @return an AllocSizeT containing the size of the derivative array in each direction
 */
AllocSizeT  GridLayout::allocSizeDerived( HybridQuantity qty, Direction dir ) const
{
    return implPtr_->allocSizeDerived( qty, dir ) ;
}



/**
 * @brief GridLayout::fieldNodeCoordinates calculate the physical coordinate
 * of a certain mesh point (ix,iy,iz) for a given quantity.
 * @return a Point centered at the field node coordinate.
 */
Point GridLayout::fieldNodeCoordinates( const Field & field, const Point & origin,
                                        uint32 ix, uint32 iy, uint32 iz  ) const
{
    return implPtr_->fieldNodeCoordinates( field, origin, ix, iy, iz ) ;
}




/**
 * @brief GridLayout::cellCenteredCoordinates calculates the physical coordinate
 * of a cell center indexed (ix,iy,iz).
 * @return a Point coordinate centered at the cell center.
 */
Point GridLayout::cellCenteredCoordinates(uint32 ix, uint32 iy, uint32 iz ) const
{
    return implPtr_->cellCenteredCoordinates( ix, iy, iz ) ;
}



QtyCentering GridLayout::fieldCentering(Field const& field, Direction dir) const
{
    return implPtr_->fieldCentering( field, dir ) ;
}



uint32 GridLayout::nbrGhostCells( QtyCentering const& centering ) const
{
    return implPtr_->nbrGhostCells( centering ) ;
}

/* ---------------------------------------------------------------------------
 *
 *                                  PRIVATE
 *
 * --------------------------------------------------------------------------- */


const std::string GridLayout::errorInverseMesh= "GridLayout error: Invalid use of";


void GridLayout::throwNotValid1D() const
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
    if (dy_ != 0. || dz_ != 0.)
        throw std::runtime_error("Error - 1D requires dy=dz=0");

    // 1D but non-zero dimensions 2 and 3.
    if ( (nbrCelly_ != 0) || (nbrCellz_ != 0) )
        throw  std::runtime_error("Error - 1D requires ny=nz=0");

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
    if (   dx_ == 0.
        || dy_ == 0.
        || dz_ == 0. )
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

