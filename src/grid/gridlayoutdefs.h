#ifndef GRIDLAYOUTDEFS_H
#define GRIDLAYOUTDEFS_H


#include "types.h"
#include "hybridenums.h"


enum class Direction{ X, Y, Z } ;

enum class QtyCentering{ primal, dual } ;




/**
 * @brief gridDataT provides constants used to initialize:
 * - hybridQuantity centerings
 * - physical start/end indexes
 * - ghost start/end indexes
 * - numbers of padding cells and physical cells
 */
struct gridDataT
{
    Direction dirX = Direction::X ;
    Direction dirY = Direction::Y ;
    Direction dirZ = Direction::Z ;

    QtyCentering primal = QtyCentering::primal ;
    QtyCentering dual   = QtyCentering::dual   ;

    uint32 idirX = static_cast<uint32>(Direction::X) ;
    uint32 idirY = static_cast<uint32>(Direction::Y) ;
    uint32 idirZ = static_cast<uint32>(Direction::Z) ;

    uint32 iBx = static_cast<uint32>(HybridQuantity::Bx) ;
    uint32 iBy = static_cast<uint32>(HybridQuantity::By) ;
    uint32 iBz = static_cast<uint32>(HybridQuantity::Bz) ;

    uint32 iEx = static_cast<uint32>(HybridQuantity::Ex) ;
    uint32 iEy = static_cast<uint32>(HybridQuantity::Ey) ;
    uint32 iEz = static_cast<uint32>(HybridQuantity::Ez) ;

    uint32 irho = static_cast<uint32>(HybridQuantity::rho) ;
    uint32 iV = static_cast<uint32>(HybridQuantity::V) ;
    uint32 iP = static_cast<uint32>(HybridQuantity::P) ;
};



#endif // GRIDLAYOUTDEFS_H
