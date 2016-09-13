#ifndef GRIDLAYOUTIMPL_H
#define GRIDLAYOUTIMPL_H

#include <array>

#include "constants.h"
#include "types.h"
#include "hybridenums.h"

#include "Field/field.h"



enum class Direction{ directionX, directionY, directionZ } ;

enum class LayoutType{ primal, dual } ;

enum class EMFieldType{ EVecField, BVecField } ;


class GridLayoutImpl
{

public:

    virtual std::vector < std::tuple < uint32, Point> >
    fieldNodeCoordinates1D( const Field & field, const Point & patchOrigin ) const = 0;

    virtual std::array<AllocSizeT, NBR_COMPO> allocSize( EMFieldType vecField ) const = 0 ;

    virtual AllocSizeT  allocSizeDerived( HybridQuantity qty, Direction dir ) const = 0 ;

    virtual std::array<AllocSizeT, NBR_COMPO> allocSize( OhmTerm term ) const = 0;

    // start and end index used in computing loops
    virtual uint32 physicalStartIndex(Field const& field, Direction direction) const = 0;
    virtual uint32 physicalEndIndex  (Field const& field, Direction direction) const = 0;

    virtual uint32 ghostStartIndex(Field const& field, Direction direction) const = 0;
    virtual uint32 ghostEndIndex  (Field const& field, Direction direction) const = 0;

    virtual void deriv1D(Field const& operand, Direction direction, Field& derivative)const = 0;

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

    double odx_ ;
    double ody_ ;
    double odz_ ;

    std::array< std::array<LayoutType,NBR_COMPO>, NBR_HYBRID_QTY > hybridQtyCentering_ ;

    std::array< std::array<uint32,NBR_COMPO>, NBR_HYBRID_QTY > physicalStartIndex_;
    std::array< std::array<uint32,NBR_COMPO>, NBR_HYBRID_QTY > physicalEndIndex_;
    std::array< std::array<uint32,NBR_COMPO>, NBR_HYBRID_QTY > ghostStartIndex_;
    std::array< std::array<uint32,NBR_COMPO>, NBR_HYBRID_QTY > ghostEndIndex_;

    void computeOffsets(uint32 ghostParameter ) ;

    double inverseSpatialStep( Direction direction ) const noexcept ;

public:

    // minimum nbr of cells in a non-invariant direction
//    static const uint32 minNbrCells = 10;

    static const uint32 defaultNbrPaddingCells = 10;

    explicit GridLayoutImplInternals(uint32 nbDims, uint32 ghostParameter,
                                     std::array<uint32,3> nbrCellsXYZ ,
                                     std::array<double,3> dxdydz      );

    LayoutType changeLayout( LayoutType layout ) const ;

    LayoutType derivedLayout( HybridQuantity qty, Direction dir) const ;

    uint32 nbrPaddingCells( Direction direction ) const noexcept;
    uint32 nbrPhysicalCells( Direction direction ) const noexcept;

    uint32 nbrGhostAtMin( LayoutType centering ) const noexcept;
    uint32 nbrGhostAtMax( LayoutType centering ) const noexcept;

    uint32 cellIndexAtMin( LayoutType centering, Direction direction ) const ;
    uint32 cellIndexAtMax( LayoutType centering, Direction direction ) const ;

    uint32 ghostCellIndexAtMax( LayoutType centering, Direction direction ) const ;
};



#endif // GRIDLAYOUTIMPL_H
