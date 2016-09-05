#include "gridlayoutimplyee.h"


GridLayoutImplYee::GridLayoutImplYee(uint32 nbDims)
    :GridLayoutImplInternals{nbDims}
{

}



uint32 GridLayoutImplYee::nx(uint32 nbrCellx) const
{
    return nbrCellx+1;
}


uint32 GridLayoutImplYee::ny(uint32 nbrCelly) const
{
    return nbrCelly+1;
}


uint32 GridLayoutImplYee::nz(uint32 nbrCellz) const
{
    return nbrCellz+1;
}



// start and end index used in computing loops
uint32 GridLayoutImplYee::physicalStartIndex(Field const& field, uint32 direction) const
{
    // which field are we working on?
    return 0;
}


uint32 GridLayoutImplYee::physicalEndIndex(Field const& field, uint32 direction) const
{
    return 0;
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



