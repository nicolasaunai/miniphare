
#include "gridlayoutimpl.h"

#include <cmath>
#include <stdexcept>


GridLayoutImplInternals::GridLayoutImplInternals(uint32 nbDims, uint32 interpOrder,
                                                 std::array<uint32,3> nbrCellsXYZ)
     : nbdims_{nbDims},
       nbrCellx_{nbrCellsXYZ[0]}, nbrCelly_{nbrCellsXYZ[1]}, nbrCellz_{nbrCellsXYZ[2]},
       nbrCellsPRA_{defaultNbrCellsPRA}
{
    computeOffsets( interpOrder );
}


void GridLayoutImplInternals::computeOffsets(uint32 interpOrder)
{
    centeredOffset_ = static_cast<uint32> ( std::floor(interpOrder/2.) )      ;
    leftOffset_     = static_cast<uint32> ( std::floor((interpOrder -1)/2.) ) ;
    rightOffset_    = static_cast<uint32> ( std::floor((interpOrder +1)/2.) ) ;
}


uint32 GridLayoutImplInternals::primalNumCellMin( uint32 direction ) const
{

    uint32 numCell = 0 ;

    // TODO : nbrCellsPRA might depend on the direction
    switch( direction )
    {
        case GridLayoutImplInternals::directionX :
            numCell = centeredOffset_ + nbrCellsPRA_ ;
            break;

        case GridLayoutImplInternals::directionY :
            numCell = centeredOffset_ + nbrCellsPRA_ ;
            break;

        case GridLayoutImplInternals::directionZ :
            numCell = centeredOffset_ + nbrCellsPRA_ ;
            break;

        default:
            throw std::runtime_error("Can't get primalNumCellMin : invalid direction");
    }

    return numCell ;
}

uint32 GridLayoutImplInternals::dualNumCellMin( uint32 direction ) const
{

    uint32 numCell = 0 ;

    // TODO : nbrCellsPRA might depend on the direction
    switch( direction )
    {
        case GridLayoutImplInternals::directionX :
            numCell = leftOffset_ + nbrCellsPRA_ ;
            break;

        case GridLayoutImplInternals::directionY :
            numCell = leftOffset_ + nbrCellsPRA_ ;
            break;

        case GridLayoutImplInternals::directionZ :
            numCell = leftOffset_ + nbrCellsPRA_ ;
            break;

        default:
            throw std::runtime_error("Can't get primalNumCellMin : invalid direction");
    }

    return numCell ;
}

uint32 GridLayoutImplInternals::primalNumCellMax( uint32 direction ) const
{
    uint32 numCell = 0 ;

    switch( direction )
    {
        case GridLayoutImplInternals::directionX :
            numCell = primalNumCellMin(direction) + nbrCellx_ ;
            break;

        case GridLayoutImplInternals::directionY :
            numCell = primalNumCellMin(direction) + nbrCelly_ ;
            break;

        case GridLayoutImplInternals::directionZ :
            numCell = primalNumCellMin(direction) + nbrCellz_ ;
            break;

        default:
            throw std::runtime_error("Can't get primalNumCellMin : invalid direction");
    }

    return numCell ;
}

uint32 GridLayoutImplInternals::dualNumCellMax( uint32 direction ) const
{
    uint32 numCell = 0 ;

    switch( direction )
    {
        case GridLayoutImplInternals::directionX :
            numCell = dualNumCellMin(direction) + nbrCellx_ ;
            break;

        case GridLayoutImplInternals::directionY :
            numCell = dualNumCellMin(direction) + nbrCelly_ ;
            break;

        case GridLayoutImplInternals::directionZ :
            numCell = dualNumCellMin(direction) + nbrCellz_ ;
            break;

        default:
            throw std::runtime_error("Can't get primalNumCellMin : invalid direction");
    }

    return numCell ;
}


