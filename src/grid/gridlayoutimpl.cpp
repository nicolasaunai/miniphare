
#include "gridlayoutimpl.h"

#include "hybridenums.h"

#include <cmath>
#include <stdexcept>


GridLayoutImplInternals::GridLayoutImplInternals(uint32 nbDims, uint32 interpOrder,
                                                 std::array<uint32,3> nbrCellsXYZ ,
                                                 std::array<double,3> dxdydz      )
    : nbdims_{nbDims},
      nbrCellx_{nbrCellsXYZ[0]}, nbrCelly_{nbrCellsXYZ[1]}, nbrCellz_{nbrCellsXYZ[2]},
      nbrPaddingCellsX_{defaultNbrPaddingCells},
      nbrPaddingCellsY_{defaultNbrPaddingCells},
      nbrPaddingCellsZ_{defaultNbrPaddingCells},
      dx_{dxdydz[0]}, dy_{dxdydz[1]}, dz_{dxdydz[2]}
{
    computeOffsets( interpOrder );
}


LayoutType GridLayoutImplInternals::derivedLayout(HybridQuantity qty, Direction dir) const
{

    uint32 iField = static_cast<uint32>( qty ) ;
    uint32 idir   = static_cast<uint32>( dir ) ;

    LayoutType newLayout = changeLayout( hybridQtyCentering_[iField][idir] ) ;

    return newLayout ;
}


LayoutType GridLayoutImplInternals::changeLayout(LayoutType layout ) const
{
    LayoutType newLayout = LayoutType::primal ;

    if( layout == LayoutType::primal )
    {
        newLayout = LayoutType::dual ;
    }

    return newLayout ;
}


void GridLayoutImplInternals::computeOffsets(uint32 interpOrder)
{
    centeredOffset_ = static_cast<uint32> ( std::floor(interpOrder/2.) )      ;
    leftOffset_     = static_cast<uint32> ( std::floor((interpOrder -1)/2.) ) ;
    rightOffset_    = static_cast<uint32> ( std::floor((interpOrder +1)/2.) ) ;
}


uint32 GridLayoutImplInternals::nbrPaddingCells( Direction direction ) const noexcept
{
    uint32 number = 0 ;

    switch( direction )
    {
    case Direction::directionX:
        number = nbrPaddingCellsX_ ;
        break ;

    case Direction::directionY:
        number = nbrPaddingCellsY_ ;
        break ;

    case Direction::directionZ:
        number = nbrPaddingCellsZ_ ;
        break ;
    }

    return number ;
}


uint32 GridLayoutImplInternals::nbrPhysicalCells( Direction direction ) const noexcept
{
    uint32 number = 0 ;

    switch( direction )
    {
    case Direction::directionX:
        number = nbrCellx_ ;
        break ;

    case Direction::directionY:
        number = nbrCelly_ ;
        break ;

    case Direction::directionZ:
        number = nbrCellz_ ;
        break ;
    }

    return number ;
}


uint32 GridLayoutImplInternals::cellIndexAtMin( LayoutType centering,
                                                Direction direction ) const
{

    uint32 numCell = nbrPaddingCells( direction ) + nbrGhostAtMin( centering );

    return numCell ;
}


uint32 GridLayoutImplInternals::cellIndexAtMax( LayoutType centering,
                                                Direction direction ) const
{
    uint32 numCell = cellIndexAtMin(centering, direction) + nbrPhysicalCells( direction ) ;

    return numCell ;
}


uint32 GridLayoutImplInternals::ghostCellIndexAtMax( LayoutType centering,
                                                     Direction direction ) const
{
    uint32 numCell = cellIndexAtMax( centering, direction ) + nbrGhostAtMax( centering );

    return numCell ;
}

uint32 GridLayoutImplInternals::nbrGhostAtMin( LayoutType centering ) const noexcept
{
    uint32 nbrGhosts = centeredOffset_ ;

    if( centering == LayoutType::dual )
    {
        nbrGhosts = leftOffset_;
    }

    return nbrGhosts ;
}

uint32 GridLayoutImplInternals::nbrGhostAtMax( LayoutType centering ) const noexcept
{
    uint32 nbrGhosts = centeredOffset_ ;

    if( centering == LayoutType::dual )
    {
        nbrGhosts = rightOffset_ ;
    }

    return nbrGhosts ;
}



