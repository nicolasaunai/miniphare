#ifndef GRIDLAYOUTIMPL_H
#define GRIDLAYOUTIMPL_H

#include "types.h"
#include "Field/field.h"

class GridLayoutImpl
{

public:


    // start and end index used in computing loops
    virtual uint32 physicalStartIndex(Field const& field, uint32 direction) const = 0;
    virtual uint32 physicalEndIndex  (Field const& field, uint32 direction) const = 0;

    virtual uint32 ghostStartIndex(Field const& field, uint32 direction) const = 0;
    virtual uint32 ghostEndIndex  (Field const& field, uint32 direction) const = 0;

    virtual void deriv(Field const& operand, uint32 direction, Field& derivative)const = 0;

    virtual uint32 nbDimensions() const = 0;

    virtual ~GridLayoutImpl() = default;

};




class GridLayoutImplInternals
{
protected:

    uint32 nbdims_;

public:
    explicit GridLayoutImplInternals(uint32 nbDims);
    uint32 nbDimensions_() const  {return nbdims_;}
};






#endif // GRIDLAYOUTIMPL_H
