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
            index = dualNumCellMin( direction ) ;
        } else
        {
            index = primalNumCellMin( direction ) ;
        }
    }
    else if( field.name().find("Ey") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionY )
        {
            index = dualNumCellMin( direction ) ;
        } else
        {
            index = primalNumCellMin( direction ) ;
        }
    }
    else if( field.name().find("Ez") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionZ )
        {
            index = dualNumCellMin( direction ) ;
        } else
        {
            index = primalNumCellMin( direction ) ;
        }
    }
    else if( field.name().find("Bx") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionX )
        {
            index = primalNumCellMin( direction ) ;
        } else
        {
            index = dualNumCellMin( direction ) ;
        }
    }
    else if( field.name().find("By") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionY )
        {
            index = primalNumCellMin( direction ) ;
        } else
        {
            index = dualNumCellMin( direction ) ;
        }
    }
    else if( field.name().find("Bz") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionZ )
        {
            index = primalNumCellMin( direction ) ;
        } else
        {
            index = dualNumCellMin( direction ) ;
        }
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
            index = dualNumCellMax( direction ) ;
        } else
        {
            index = primalNumCellMax( direction ) ;
        }
    }
    else if( field.name().find("Ey") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionY )
        {
            index = dualNumCellMax( direction ) ;
        } else
        {
            index = primalNumCellMax( direction ) ;
        }
    }
    else if( field.name().find("Ez") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionZ )
        {
            index = dualNumCellMax( direction ) ;
        } else
        {
            index = primalNumCellMax( direction ) ;
        }
    }
    else if( field.name().find("Bx") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionX )
        {
            index = primalNumCellMax( direction ) ;
        } else
        {
            index = dualNumCellMax( direction ) ;
        }
    }
    else if( field.name().find("By") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionY )
        {
            index = primalNumCellMax( direction ) ;
        } else
        {
            index = dualNumCellMax( direction ) ;
        }
    }
    else if( field.name().find("Bz") != std::string::npos )
    {
        if( direction == GridLayoutImplInternals::directionZ )
        {
            index = primalNumCellMax( direction ) ;
        } else
        {
            index = dualNumCellMax( direction ) ;
        }
    }
    else
    {
        throw std::runtime_error("Can't get physicalStartIndex : wrong field");
    }


    return index ;
}



uint32 GridLayoutImplYee::ghostStartIndex(Field const& field, uint32 direction) const
{
    return 0;
}


uint32 GridLayoutImplYee::ghostEndIndex(Field const& field, uint32 direction) const
{
    return 0;
}


void GridLayoutImplYee::deriv(Field const& operand, uint32 direction, Field& derivative) const
{
}



