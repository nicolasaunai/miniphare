
#include "gridlayoutimpl.h"

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


uint32 GridLayoutImplInternals::nbrPaddingCells( uint32 direction ) const
{
    uint32 number = 0 ;

    switch( direction )
    {
    case GridLayoutImplInternals::directionX:
        number = nbrPaddingCellsX_ ;
        break ;

    case GridLayoutImplInternals::directionY:
        number = nbrPaddingCellsY_ ;
        break ;

    case GridLayoutImplInternals::directionZ:
        number = nbrPaddingCellsZ_ ;
        break ;

    default:
        throw std::runtime_error("Can't get nbrPaddingCells : invalid direction");
    }

    return number ;
}


uint32 GridLayoutImplInternals::nbrPhysicalCells( uint32 direction ) const
{
    uint32 number = 0 ;

    switch( direction )
    {
    case GridLayoutImplInternals::directionX:
        number = nbrCellx_ ;
        break ;

    case GridLayoutImplInternals::directionY:
        number = nbrCelly_ ;
        break ;

    case GridLayoutImplInternals::directionZ:
        number = nbrCellz_ ;
        break ;

    default:
        throw std::runtime_error("Can't get nbrPhysicalCells : invalid direction");
    }

    return number ;
}


uint32 GridLayoutImplInternals::primalCellNbrMin( uint32 direction ) const
{
    uint32 numCell = centeredOffset_ + nbrPaddingCells( direction ) ;

    return numCell ;
}

uint32 GridLayoutImplInternals::dualCellNbrMin( uint32 direction ) const
{
    uint32 numCell = leftOffset_ + nbrPaddingCells( direction ) ;

    return numCell ;
}

uint32 GridLayoutImplInternals::primalCellNbrMax( uint32 direction ) const
{
    uint32 numCell = primalCellNbrMin(direction) + nbrPhysicalCells( direction ) ;

    return numCell ;
}

uint32 GridLayoutImplInternals::dualCellNbrMax( uint32 direction ) const
{
    uint32 numCell = dualCellNbrMin(direction) + nbrPhysicalCells( direction ) ;

    return numCell ;
}



uint32 GridLayoutImplInternals::primalGhostCellNbrMax( uint32 direction ) const
{
    uint32 numCell = primalCellNbrMax( direction ) + centeredOffset_ ;

    return numCell ;
}


uint32 GridLayoutImplInternals::dualGhostCellNbrMax( uint32 direction ) const
{
    uint32 numCell = dualCellNbrMax( direction ) + rightOffset_ ;

    return numCell ;
}




