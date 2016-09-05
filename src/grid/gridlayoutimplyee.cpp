#include <stdexcept>

#include "gridlayoutimplyee.h"




GridLayoutImplYee::GridLayoutImplYee(uint32 nbDims,
                                     uint32 interpOrder,
                                     std::array<uint32,3> nbrCellsXYZ)
    : GridLayoutImplInternals(nbDims, interpOrder, nbrCellsXYZ)
{

}



uint32 GridLayoutImplYee::nx(uint32 nbrCellx) const
{
    return 0;
}


uint32 GridLayoutImplYee::ny(uint32 nbrCelly) const
{
    return 0;
}


uint32 GridLayoutImplYee::nz(uint32 nbrCellz) const
{
    return 0;
}



// start and end index used in computing loops
uint32 GridLayoutImplYee::physicalStartIndex(Field const& field, uint32 direction) const
{
    uint32 index = 0 ; //the value we are looking for

    // which field are we working on?
    if( field.name().find("Ex") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionX )
        {
            index = dualCellNbrMin( direction ) ;
        } else
        {
            index = primalCellNbrMin( direction ) ;
        }
    }
    else if( field.name().find("Ey") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionY )
        {
            index = dualCellNbrMin( direction ) ;
        } else
        {
            index = primalCellNbrMin( direction ) ;
        }
    }
    else if( field.name().find("Ez") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionZ )
        {
            index = dualCellNbrMin( direction ) ;
        } else
        {
            index = primalCellNbrMin( direction ) ;
        }
    }
    else if( field.name().find("Bx") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionX )
        {
            index = primalCellNbrMin( direction ) ;
        } else
        {
            index = dualCellNbrMin( direction ) ;
        }
    }
    else if( field.name().find("By") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionY )
        {
            index = primalCellNbrMin( direction ) ;
        } else
        {
            index = dualCellNbrMin( direction ) ;
        }
    }
    else if( field.name().find("Bz") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionZ )
        {
            index = primalCellNbrMin( direction ) ;
        } else
        {
            index = dualCellNbrMin( direction ) ;
        }
    }
    else if( field.name().find("rho") != std::string::npos )
    {
        index = primalCellNbrMin( direction ) ;
    }
    else if( field.name().find("Jx") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionX )
        {
            index = dualCellNbrMin( direction ) ;
        } else
        {
            index = primalCellNbrMin( direction ) ;
        }
    }
    else if( field.name().find("Jy") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionY )
        {
            index = dualCellNbrMin( direction ) ;
        } else
        {
            index = primalCellNbrMin( direction ) ;
        }
    }
    else if( field.name().find("Jz") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionZ )
        {
            index = dualCellNbrMin( direction ) ;
        } else
        {
            index = primalCellNbrMin( direction ) ;
        }
    }
    else if( field.name().find("Pxx") != std::string::npos )
    {
        index = primalCellNbrMin( direction ) ;
    }
    else if( field.name().find("Pxy") != std::string::npos )
    {
        index = primalCellNbrMin( direction ) ;
    }
    else if( field.name().find("Pxz") != std::string::npos )
    {
        index = primalCellNbrMin( direction ) ;
    }
    else if( field.name().find("Pyy") != std::string::npos )
    {
        index = primalCellNbrMin( direction ) ;
    }
    else if( field.name().find("Pyz") != std::string::npos )
    {
        index = primalCellNbrMin( direction ) ;
    }
    else if( field.name().find("Pzz") != std::string::npos )
    {
        index = primalCellNbrMin( direction ) ;
    }
    else
    {
        throw std::runtime_error("Can't get physicalStartIndex : wrong field");
    }


    return index ;
}


uint32 GridLayoutImplYee::physicalEndIndex(Field const& field, uint32 direction) const
{
    uint32 index = 0 ; //the value we are looking for

    // which field are we working on?
    if( field.name().find("Ex") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionX )
        {
            index = dualCellNbrMax( direction ) ;
        } else
        {
            index = primalCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Ey") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionY )
        {
            index = dualCellNbrMax( direction ) ;
        } else
        {
            index = primalCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Ez") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionZ )
        {
            index = dualCellNbrMax( direction ) ;
        } else
        {
            index = primalCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Bx") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionX )
        {
            index = primalCellNbrMax( direction ) ;
        } else
        {
            index = dualCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("By") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionY )
        {
            index = primalCellNbrMax( direction ) ;
        } else
        {
            index = dualCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Bz") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionZ )
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
        if( direction == GridLayoutImplInternals::directionX )
        {
            index = dualCellNbrMax( direction ) ;
        } else
        {
            index = primalCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Jy") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionY )
        {
            index = dualCellNbrMax( direction ) ;
        } else
        {
            index = primalCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Jz") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionZ )
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



uint32 GridLayoutImplYee::ghostStartIndex(Field const& field, uint32 direction) const
{

    return  nbrPaddingCells( direction ) ;
}


uint32 GridLayoutImplYee::ghostEndIndex(Field const& field, uint32 direction) const
{

    uint32 index = 0 ; //the value we are looking for

    // which field are we working on?
    if( field.name().find("Ex") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionX )
        {
            index = dualGhostCellNbrMax( direction ) ;
        } else
        {
            index = primalGhostCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Ey") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionY )
        {
            index = dualGhostCellNbrMax( direction ) ;
        } else
        {
            index = primalGhostCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Ez") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionZ )
        {
            index = dualGhostCellNbrMax( direction ) ;
        } else
        {
            index = primalGhostCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Bx") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionX )
        {
            index = primalGhostCellNbrMax( direction ) ;
        } else
        {
            index = dualGhostCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("By") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionY )
        {
            index = primalGhostCellNbrMax( direction ) ;
        } else
        {
            index = dualGhostCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Bz") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionZ )
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
        if( direction == GridLayoutImplInternals::directionX )
        {
            index = dualGhostCellNbrMax( direction ) ;
        } else
        {
            index = primalGhostCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Jy") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionY )
        {
            index = dualGhostCellNbrMax( direction ) ;
        } else
        {
            index = primalGhostCellNbrMax( direction ) ;
        }
    }
    else if( field.name().find("Jz") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionZ )
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


void GridLayoutImplYee::deriv(Field const& operand, uint32 direction, Field& derivative) const
{
}



