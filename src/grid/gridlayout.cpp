#include "gridlayout.h"


// start and end index used in computing loops
uint32 GridLayout::physicalStartIndex(Field const& field, uint32 direction) const
{
    return 0;
}


uint32 GridLayout::physicalEndIndex(Field const& field, uint32 direction) const
{
return 0;
}



uint32 GridLayout::ghostStartIndex(Field const& field, uint32 direction) const
{
return 0;
}

uint32 GridLayout::ghostEndIndex(Field const& field, uint32 direction) const
{
return 0;
}


void GridLayout::deriv(Field const& operand, uint32 direction, Field& derivative) const
{

}
