
#include "gridlayoutimpl.h"

#include "gridconstants.h"

#include <cmath>
#include <stdexcept>


GridLayoutImplInternals::GridLayoutImplInternals(uint32 nbDims, uint32 interpOrder,
                                                 std::array<uint32,3> nbrCellsXYZ)
     : nbdims_{nbDims},
       nbrCellx_{nbrCellsXYZ[0]}, nbrCelly_{nbrCellsXYZ[1]}, nbrCellz_{nbrCellsXYZ[2]},
       nbrPaddingCellsX_{defaultNbrPaddingCells},
       nbrPaddingCellsY_{defaultNbrPaddingCells},
       nbrPaddingCellsZ_{defaultNbrPaddingCells}
{
    computeOffsets( interpOrder );
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


uint32 GridLayoutImplInternals::primalCellNbrMin( Direction direction ) const
{
    uint32 numCell = centeredOffset_ + nbrPaddingCells( direction ) ;

    return numCell ;
}

uint32 GridLayoutImplInternals::dualCellNbrMin( Direction direction ) const
{
    uint32 numCell = leftOffset_ + nbrPaddingCells( direction ) ;

    return numCell ;
}

uint32 GridLayoutImplInternals::primalCellNbrMax( Direction direction ) const
{
    uint32 numCell = primalCellNbrMin(direction) + nbrPhysicalCells( direction ) ;

    return numCell ;
}

uint32 GridLayoutImplInternals::dualCellNbrMax( Direction direction ) const
{
    uint32 numCell = dualCellNbrMin(direction) + nbrPhysicalCells( direction ) ;

    return numCell ;
}



uint32 GridLayoutImplInternals::primalGhostCellNbrMax( Direction direction ) const
{
    uint32 numCell = primalCellNbrMax( direction ) + centeredOffset_ ;

    return numCell ;
}


uint32 GridLayoutImplInternals::dualGhostCellNbrMax( Direction direction ) const
{
    uint32 numCell = dualCellNbrMax( direction ) + rightOffset_ ;

    return numCell ;
}




