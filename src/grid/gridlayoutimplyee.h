#ifndef GRIDLAYOUTYEE_H
#define GRIDLAYOUTYEE_H


#include <vector>
#include <array>

#include "gridlayoutimpl.h"
#include "types.h"




class GridLayoutImplYee : public GridLayoutImpl, private GridLayoutImplInternals
{
private:



public:

    explicit GridLayoutImplYee(uint32 nbDims,
                               uint32 interpOrder,
                               std::array<uint32,3> nbrCellsXYZ);
    virtual ~GridLayoutImplYee() = default;


    virtual uint32 nbDimensions() const override {return nbDimensions_();}

    virtual uint32 allocSizeX( std::string fieldName, uint32 direction ) const override;
    virtual uint32 allocSizeY( std::string fieldName, uint32 direction ) const override;
    virtual uint32 allocSizeZ( std::string fieldName, uint32 direction ) const override;

    // start and end index used in computing loops
    virtual uint32 physicalStartIndex(Field const& field, uint32 direction) const override;
    virtual uint32 physicalEndIndex  (Field const& field, uint32 direction) const override;

    virtual uint32 ghostStartIndex(Field const& field, uint32 direction) const override;
    virtual uint32 ghostEndIndex  (Field const& field, uint32 direction) const override;

    virtual void deriv(Field const& operand, uint32 direction, Field& derivative) const override;


};



#endif // GRIDLAYOUTYEE_H
