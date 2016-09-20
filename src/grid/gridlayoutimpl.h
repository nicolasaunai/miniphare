#ifndef GRIDLAYOUTIMPL_H
#define GRIDLAYOUTIMPL_H

#include <array>

#include "constants.h"
#include "types.h"
#include "hybridenums.h"

#include "Field/field.h"



enum class Direction{ X, Y, Z } ;

enum class QtyCentering{ primal, dual } ;

enum class EMFieldType{ EVecField, BVecField } ;




struct gridDataT
{
    Direction dirX = Direction::X ;
    Direction dirY = Direction::Y ;
    Direction dirZ = Direction::Z ;

    QtyCentering primal = QtyCentering::primal ;
    QtyCentering dual   = QtyCentering::dual   ;

    uint32 idirX = static_cast<uint32>(Direction::X) ;
    uint32 idirY = static_cast<uint32>(Direction::Y) ;
    uint32 idirZ = static_cast<uint32>(Direction::Z) ;

    uint32 iBx = static_cast<uint32>(HybridQuantity::Bx) ;
    uint32 iBy = static_cast<uint32>(HybridQuantity::By) ;
    uint32 iBz = static_cast<uint32>(HybridQuantity::Bz) ;

    uint32 iEx = static_cast<uint32>(HybridQuantity::Ex) ;
    uint32 iEy = static_cast<uint32>(HybridQuantity::Ey) ;
    uint32 iEz = static_cast<uint32>(HybridQuantity::Ez) ;

    uint32 irho = static_cast<uint32>(HybridQuantity::rho) ;
    uint32 iV = static_cast<uint32>(HybridQuantity::V) ;
    uint32 iP = static_cast<uint32>(HybridQuantity::P) ;
};



using gridCoordinate = std::vector< std::tuple <uint32, Point> >;






class GridLayoutImpl
{

public:

    virtual Point fieldNodeCoordinates(
            const Field & field, const Point & origin,
            uint32 ix, uint32 iy, uint32 iz ) const = 0;

    virtual Point cellCenteredCoordinates(
            const Point & origin, uint32 ix, uint32 iy, uint32 iz ) const = 0 ;

    virtual AllocSizeT allocSize( HybridQuantity qtyType ) const = 0 ;

    virtual AllocSizeT  allocSizeDerived( HybridQuantity qty, Direction dir ) const = 0 ;

    // start and end index used in computing loops
    virtual uint32 physicalStartIndex(Field const& field, Direction direction) const = 0;
    virtual uint32 physicalEndIndex  (Field const& field, Direction direction) const = 0;

    virtual uint32 ghostStartIndex(Field const& field, Direction direction) const = 0;
    virtual uint32 ghostEndIndex  (Field const& field, Direction direction) const = 0;

    virtual void deriv1D(Field const& operand, Field& derivative)const = 0;
    //virtual void deriv2D(Field const& operand, Direction direction, Field& derivative)const = 0;
    //virtual void deriv3D(Field const& operand, Direction direction, Field& derivative)const = 0;

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

    uint32 nbrPrimalGhosts_ ;
    uint32 nbrDualGhosts_ ;

    uint32 nbrPaddingCellsX_ ;
    uint32 nbrPaddingCellsY_ ;
    uint32 nbrPaddingCellsZ_ ;

    double dx_ ;
    double dy_ ;
    double dz_ ;


    std::array<double, NBR_COMPO> odxdydz_ ;

    std::array<uint32, NBR_COMPO> nbrPaddingCells_  ;
    std::array<uint32, NBR_COMPO> nbrPhysicalCells_ ;

    std::array< std::array<QtyCentering,NBR_COMPO>, NBR_HYBRID_QTY > hybridQtyCentering_ ;

    std::array< std::array<uint32,NBR_COMPO>, NBR_HYBRID_QTY > physicalStartIndex_;
    std::array< std::array<uint32,NBR_COMPO>, NBR_HYBRID_QTY > physicalEndIndex_;
    std::array< std::array<uint32,NBR_COMPO>, NBR_HYBRID_QTY > ghostStartIndex_;
    std::array< std::array<uint32,NBR_COMPO>, NBR_HYBRID_QTY > ghostEndIndex_;

    void computeNbrGhosts(uint32 ghostParameter ) ;

    double inverseSpatialStep( Direction direction ) const noexcept ;

public:

    // minimum nbr of cells in a non-invariant direction
    //    static const uint32 minNbrCells = 10;

    static const uint32 defaultNbrPaddingCells = 0;//10;

    GridLayoutImplInternals(uint32 nbDims, uint32 ghostParameter,
                            std::array<uint32,3> nbrCellsXYZ ,
                            std::array<double,3> dxdydz      );

    void initGridUtils( const gridDataT & staticData ) ;

    void initPhysicalStart( const gridDataT & staticData ) ;
    void initPhysicalEnd  ( const gridDataT & staticData ) ;
    void initGhostStart( const gridDataT & staticData ) ;
    void initGhostEnd  ( const gridDataT & staticData ) ;


    AllocSizeT allocSize_( HybridQuantity qty ) const;
    AllocSizeT  allocSizeDerived_( HybridQuantity qty, Direction dir ) const;

    // start and end index used in computing loops
    uint32 physicalStartIndexV(Field const& field, Direction direction) const;
    uint32 physicalEndIndexV  (Field const& field, Direction direction) const;
    uint32 ghostStartIndexV   (Field const& field, Direction direction) const;
    uint32 ghostEndIndexV     (Field const& field, Direction direction) const;

    QtyCentering changeCentering( QtyCentering layout ) const ;
    QtyCentering derivedCentering( HybridQuantity qty, Direction dir) const ;

    uint32 nbrPaddingCells( Direction direction ) const noexcept;
    uint32 nbrPhysicalCells( Direction direction ) const noexcept;

    uint32 nbrGhosts( QtyCentering centering ) const noexcept;
    uint32 isDual( QtyCentering centering ) const noexcept;

    uint32 cellIndexAtMin( QtyCentering centering, Direction direction ) const ;
    uint32 cellIndexAtMax( QtyCentering centering, Direction direction ) const ;

    uint32 ghostCellIndexAtMax( QtyCentering centering, Direction direction ) const ;
};



#endif // GRIDLAYOUTIMPL_H
