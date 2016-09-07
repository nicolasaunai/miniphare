#include <stdexcept>

#include "gridlayoutimplyee.h"




GridLayoutImplYee::GridLayoutImplYee(uint32 nbDims,
                                     uint32 interpOrder,
                                     std::array<uint32,3> nbrCellsXYZ)
    : GridLayoutImplInternals(nbDims, interpOrder, nbrCellsXYZ)
{
    Direction dirX = Direction::directionX ;
    Direction dirY = Direction::directionY ;
    Direction dirZ = Direction::directionZ ;

    uint32 idirX = static_cast<uint32>(Direction::directionX) ;
    uint32 idirY = static_cast<uint32>(Direction::directionY) ;
    uint32 idirZ = static_cast<uint32>(Direction::directionZ) ;

    uint32 iBx = static_cast<uint32>(HybridQuantity::Bx) ;
    uint32 iBy = static_cast<uint32>(HybridQuantity::By) ;
    uint32 iBz = static_cast<uint32>(HybridQuantity::Bz) ;

    uint32 iEx = static_cast<uint32>(HybridQuantity::Ex) ;
    uint32 iEy = static_cast<uint32>(HybridQuantity::Ey) ;
    uint32 iEz = static_cast<uint32>(HybridQuantity::Ez) ;

    uint32 irho = static_cast<uint32>(HybridQuantity::rho) ;
    uint32 iV = static_cast<uint32>(HybridQuantity::V) ;
    uint32 iP = static_cast<uint32>(HybridQuantity::P) ;

    physicalStartIndex_[iBx][idirX] = primalCellNbrMin( dirX ) ;
    physicalStartIndex_[iBy][idirX] = dualCellNbrMin  ( dirX ) ;
    physicalStartIndex_[iBz][idirX] = dualCellNbrMin  ( dirX ) ;

    physicalStartIndex_[iBx][idirY] = dualCellNbrMin  ( dirY ) ;
    physicalStartIndex_[iBy][idirY] = primalCellNbrMin( dirY ) ;
    physicalStartIndex_[iBz][idirY] = dualCellNbrMin  ( dirY ) ;

    physicalStartIndex_[iBx][idirZ] = dualCellNbrMin  ( dirZ ) ;
    physicalStartIndex_[iBy][idirZ] = dualCellNbrMin  ( dirZ ) ;
    physicalStartIndex_[iBz][idirZ] = primalCellNbrMin( dirZ ) ;

    physicalStartIndex_[iEx][idirX] = dualCellNbrMin  ( dirX ) ;
    physicalStartIndex_[iEy][idirX] = primalCellNbrMin( dirX ) ;
    physicalStartIndex_[iEz][idirX] = primalCellNbrMin( dirX ) ;

    physicalStartIndex_[iEx][idirY] = primalCellNbrMin( dirY ) ;
    physicalStartIndex_[iEy][idirY] = dualCellNbrMin  ( dirY ) ;
    physicalStartIndex_[iEz][idirY] = primalCellNbrMin( dirY ) ;

    physicalStartIndex_[iEx][idirZ] = primalCellNbrMin( dirZ ) ;
    physicalStartIndex_[iEy][idirZ] = primalCellNbrMin( dirZ ) ;
    physicalStartIndex_[iEz][idirZ] = dualCellNbrMin  ( dirZ ) ;

    physicalStartIndex_[irho][idirX] = primalCellNbrMin( dirX ) ;
    physicalStartIndex_[iV][idirX]   = primalCellNbrMin( dirX ) ;
    physicalStartIndex_[iP][idirX]   = primalCellNbrMin( dirX ) ;

    physicalStartIndex_[irho][idirY] = primalCellNbrMin( dirY ) ;
    physicalStartIndex_[iV][idirY]   = primalCellNbrMin( dirY ) ;
    physicalStartIndex_[iP][idirY]   = primalCellNbrMin( dirY ) ;

    physicalStartIndex_[irho][idirZ] = primalCellNbrMin( dirZ ) ;
    physicalStartIndex_[iV][idirZ]   = primalCellNbrMin( dirZ ) ;
    physicalStartIndex_[iP][idirZ]   = primalCellNbrMin( dirZ ) ;


}


std::array<AllocSizeT ,3> GridLayoutImplYee::allocSize( EMFieldType fieldType ) const
{



}

std::array<AllocSizeT ,3> GridLayoutImplYee::allocSize( OhmTerm term ) const
{


}

AllocSizeT GridLayoutImplYee::allocSize( DerivedEMField derivedField ) const
{



}



// start and end index used in computing loops
uint32 GridLayoutImplYee::physicalStartIndex(Field const& field, Direction direction) const
{
    uint32 index = 0 ; //the value we are looking for

    return physicalStartIndex_[static_cast<uint32>(field.type())][static_cast<uint32>(direction)];

}


uint32 GridLayoutImplYee::physicalEndIndex(Field const& field, Direction direction) const
{
    uint32 index = 0 ; //the value we are looking for

    // which field are we working on?
    if( field.name().find("Ex") != std::string::npos )
    {
        if( direction == Direction::directionX )
        {
            index = dualCellNbrMax( direction ) ;
        } else
        {
            index = primalCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Ey") != std::string::npos )
    {
        if( direction == Direction::directionY )
        {
            index = dualCellNbrMax( direction ) ;
        } else
        {
            index = primalCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Ez") != std::string::npos )
    {
        if( direction == Direction::directionZ )
        {
            index = dualCellNbrMax( direction ) ;
        } else
        {
            index = primalCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Bx") != std::string::npos )
    {
        if( direction == Direction::directionX )
        {
            index = primalCellNbrMax( direction ) ;
        } else
        {
            index = dualCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("By") != std::string::npos )
    {
        if( direction == Direction::directionY )
        {
            index = primalCellNbrMax( direction ) ;
        } else
        {
            index = dualCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Bz") != std::string::npos )
    {
        if( direction == Direction::directionZ )
        {
            index = primalCellNbrMax( direction ) ;
        } else
        {
            index = dualCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("rho") != std::string::npos )
    {
        index = primalCellNbrMax( direction ) ;
    }
    else if( field.name().find("Jx") != std::string::npos )
    {
        if( direction == Direction::directionX )
        {
            index = dualCellNbrMax( direction ) ;
        } else
        {
            index = primalCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Jy") != std::string::npos )
    {
        if( direction == Direction::directionY )
        {
            index = dualCellNbrMax( direction ) ;
        } else
        {
            index = primalCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Jz") != std::string::npos )
    {
        if( direction == Direction::directionZ )
        {
            index = dualCellNbrMax( direction ) ;
        } else
        {
            index = primalCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Pxx") != std::string::npos )
    {
        index = primalCellNbrMax( direction ) ;
    }
    else if( field.name().find("Pxy") != std::string::npos )
    {
        index = primalCellNbrMax( direction ) ;
    }
    else if( field.name().find("Pxz") != std::string::npos )
    {
        index = primalCellNbrMax( direction ) ;
    }
    else if( field.name().find("Pyy") != std::string::npos )
    {
        index = primalCellNbrMax( direction ) ;
    }
    else if( field.name().find("Pyz") != std::string::npos )
    {
        index = primalCellNbrMax( direction ) ;
    }
    else if( field.name().find("Pzz") != std::string::npos )
    {
        index = primalCellNbrMax( direction ) ;
    }
    else
    {
        throw std::runtime_error("Can't get physicalEndIndex : wrong field");
    }


    return index ;
}



uint32 GridLayoutImplYee::ghostStartIndex(Field const& field, Direction direction) const
{

    return  nbrPaddingCells( direction ) ;
}


uint32 GridLayoutImplYee::ghostEndIndex(Field const& field, Direction direction) const
{

    uint32 index = 0 ; //the value we are looking for

    // which field are we working on?
    if( field.name().find("Ex") != std::string::npos )
    {
        if( direction == Direction::directionX )
        {
            index = dualGhostCellNbrMax( direction ) ;
        } else
        {
            index = primalGhostCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Ey") != std::string::npos )
    {
        if( direction == Direction::directionY )
        {
            index = dualGhostCellNbrMax( direction ) ;
        } else
        {
            index = primalGhostCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Ez") != std::string::npos )
    {
        if( direction == Direction::directionZ )
        {
            index = dualGhostCellNbrMax( direction ) ;
        } else
        {
            index = primalGhostCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Bx") != std::string::npos )
    {
        if( direction == Direction::directionX )
        {
            index = primalGhostCellNbrMax( direction ) ;
        } else
        {
            index = dualGhostCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("By") != std::string::npos )
    {
        if( direction == Direction::directionY )
        {
            index = primalGhostCellNbrMax( direction ) ;
        } else
        {
            index = dualGhostCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Bz") != std::string::npos )
    {
        if( direction == Direction::directionZ )
        {
            index = primalGhostCellNbrMax( direction ) ;
        } else
        {
            index = dualGhostCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("rho") != std::string::npos )
    {
        index = primalGhostCellNbrMax( direction ) ;
    }
    else if( field.name().find("Jx") != std::string::npos )
    {
        if( direction == Direction::directionX )
        {
            index = dualGhostCellNbrMax( direction ) ;
        } else
        {
            index = primalGhostCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Jy") != std::string::npos )
    {
        if( direction == Direction::directionY )
        {
            index = dualGhostCellNbrMax( direction ) ;
        } else
        {
            index = primalGhostCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Jz") != std::string::npos )
    {
        if( direction == Direction::directionZ )
        {
            index = dualGhostCellNbrMax( direction ) ;
        } else
        {
            index = primalGhostCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Pxx") != std::string::npos )
    {
        index = primalGhostCellNbrMax( direction ) ;
    }
    else if( field.name().find("Pxy") != std::string::npos )
    {
        index = primalGhostCellNbrMax( direction ) ;
    }
    else if( field.name().find("Pxz") != std::string::npos )
    {
        index = primalGhostCellNbrMax( direction ) ;
    }
    else if( field.name().find("Pyy") != std::string::npos )
    {
        index = primalGhostCellNbrMax( direction ) ;
    }
    else if( field.name().find("Pyz") != std::string::npos )
    {
        index = primalGhostCellNbrMax( direction ) ;
    }
    else if( field.name().find("Pzz") != std::string::npos )
    {
        index = primalGhostCellNbrMax( direction ) ;
    }
    else
    {
        throw std::runtime_error("Can't get physicalEndIndex : wrong field");
    }


    return index ;
}


void GridLayoutImplYee::deriv(Field const& operand, Direction direction, Field& derivative) const
{
}



