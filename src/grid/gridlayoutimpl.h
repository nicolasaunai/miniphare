#ifndef GRIDLAYOUTIMPL_H
#define GRIDLAYOUTIMPL_H

#include <array>

#include "types.h"
#include "gridconstants.h"

#include "Field/field.h"

class GridLayoutImpl
{

public:

    virtual std::vector < std::tuple < uint32, Point> >
    fieldNodeCoordinates1D( const Field & field, const Point & patchOrigin ) const = 0;

    virtual std::array<AllocSizeT ,3> allocSize( EMFieldType vecField ) const = 0 ;

    virtual AllocSizeT  allocSize( LayoutType layout ) const = 0 ;


    // start and end index used in computing loops
    virtual uint32 physicalStartIndex(Field const& field, Direction direction) const = 0;
    virtual uint32 physicalEndIndex  (Field const& field, Direction direction) const = 0;

    virtual uint32 ghostStartIndex(Field const& field, Direction direction) const = 0;
    virtual uint32 ghostEndIndex  (Field const& field, Direction direction) const = 0;

    virtual void deriv(Field const& operand, Direction direction, Field& derivative)const = 0;

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

    uint32 nbrPaddingCellsX_ ;
    uint32 nbrPaddingCellsY_ ;
    uint32 nbrPaddingCellsZ_ ;

    double dx_ ;
    double dy_ ;
    double dz_ ;

    void computeOffsets( uint32 interpOrder ) ;

public:

    // minimum nbr of cells in a non-invariant direction
//    static const uint32 minNbrCells = 10;

    static const uint32 defaultNbrPaddingCells = 10;

    std::array< std::array<LayoutType,3>, 9 > hybridQtyCentering_ ;

    std::array< std::array<uint32,3>, 9 > physicalStartIndex_;
    std::array< std::array<uint32,3>, 9 > physicalEndIndex_;
    std::array< std::array<uint32,3>, 9 > ghostStartIndex_;
    std::array< std::array<uint32,3>, 9 > ghostEndIndex_;


    explicit GridLayoutImplInternals(uint32 nbDims, uint32 interpOrder,
                                     std::array<uint32,3> nbrCellsXYZ ,
                                     std::array<double,3> dxdydz      );

    LayoutType changeLayout( const LayoutType & layout ) const ;

    LayoutType derivedLayout( const HybridQuantity & qty, Direction dir) const ;

    uint32 nbDimensions_() const { return nbdims_; }

    uint32 nbrPaddingCells( Direction direction ) const noexcept;
    uint32 nbrPhysicalCells( Direction direction ) const noexcept;

    uint32 cellNbrMin( LayoutType centering, Direction direction ) const ;
    uint32 cellNbrMax( LayoutType centering, Direction direction ) const ;

    uint32 ghostCellNbrMax( LayoutType centering, Direction direction ) const ;
};



#endif // GRIDLAYOUTIMPL_H
