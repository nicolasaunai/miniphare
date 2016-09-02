#ifndef GRIDLAYOUTIMPL_H
#define GRIDLAYOUTIMPL_H

#include <array>

#include "types.h"
#include "Field/field.h"

class GridLayoutImpl
{

public:


    virtual uint32 nx(uint32 nbrCellx) const = 0;
    virtual uint32 ny(uint32 nbrCelly) const = 0;
    virtual uint32 nz(uint32 nbrCellz) const = 0;

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

    uint32 nbrCellx_  ;
    uint32 nbrCelly_  ;
    uint32 nbrCellz_  ;

    uint32 centeredOffset_ ;
    uint32 leftOffset_ ;
    uint32 rightOffset_ ;

    uint32 nbrCellsPRA_ ;

    void computeOffsets( uint32 interpOrder ) ;

public:

    static const uint32 directionX = 0;
    static const uint32 directionY = 1;
    static const uint32 directionZ = 2;


    static const uint32 minNbrCells = 10; // minimum nbr of cells in a
                                          // non-invariant direction

    static const uint32 defaultNbrCellsPRA = 10;

    explicit GridLayoutImplInternals(uint32 nbDims, uint32 interpOrder,
                                     std::array<uint32,3> nbrCellsXYZ);

    uint32 nbDimensions_() const { return nbdims_; }


    uint32 primalNumCellMin( uint32 direction ) const ;
    uint32 primalNumCellMax( uint32 direction ) const ;
    uint32 dualNumCellMin( uint32 direction ) const ;
    uint32 dualNumCellMax( uint32 direction ) const ;
};






#endif // GRIDLAYOUTIMPL_H
