#ifndef GRIDLAYOUTIMPLINTERNALS_H
#define GRIDLAYOUTIMPLINTERNALS_H


#include <array>

#include "Field/field.h"
#include "types.h"
#include "hybridenums.h"
#include "constants.h"
#include "gridlayoutdefs.h"


/**
 * @brief GridLayoutImplInternals is intended to factorize attributes and methods
 * common to all GridLayoutImpl derived classes (ex: GridLayoutImplYee).
 *
 * Most of the implementations needed to handle GridLayout operations are provided
 * by GridLayoutImplInternals' methods.
 *
 */
class GridLayoutImplInternals
{

private:

    std::array<uint32, NBR_COMPO>
    nodeNbrFromCentering_( std::array<QtyCentering, NBR_COMPO> const & qtyCenterings ) const;




protected:

    uint32 nbdims_;

    uint32 nbrCellx_  ;
    uint32 nbrCelly_  ;
    uint32 nbrCellz_  ;

    uint32 nbrPrimalGhosts_ ;
    uint32 nbrDualGhosts_ ;

    double dx_ ;
    double dy_ ;
    double dz_ ;

    std::array<double, NBR_COMPO> odxdydz_ ;

    std::array<uint32, NBR_COMPO> nbrPhysicalCells_ ;

    std::array< std::array<QtyCentering,NBR_COMPO>, NBR_HYBRID_QTY > hybridQtyCentering_ ;

    std::array< std::array<uint32, 3>, 2 > physicalStartIndexTable_;
    std::array< std::array<uint32, 3>, 2 > physicalEndIndexTable_;
    std::array< std::array<uint32, 3>, 2 > ghostEndIndexTable_;





    void computeNbrGhosts(uint32 ghostParameter ) ;

    double inverseSpatialStep( Direction direction ) const noexcept ;

    // start and end index used in computing loops
    uint32 physicalStartIndex_(Field const& field, Direction direction) const;
    uint32 physicalStartIndex_(QtyCentering centering, Direction direction) const;

    uint32 physicalEndIndex_  (Field const& field, Direction direction) const;
    uint32 physicalEndIndex_  (QtyCentering centering, Direction direction) const;

    uint32 ghostStartIndex_   (Field const& field, Direction direction) const;
    uint32 ghostEndIndex_     (Field const& field, Direction direction) const;



    AllocSizeT allocSize_( HybridQuantity qty ) const;
    AllocSizeT allocSizeDerived_( HybridQuantity qty, Direction dir ) const;


    Point fieldNodeCoordinates_( const Field & field, const Point & origin,
                                 uint32 ix, uint32 iy, uint32 iz ) const;

    Point cellCenteredCoordinates_(const Point & origin, uint32 ix, uint32 iy, uint32 iz ) const;




//public:

    // minimum nbr of cells in a non-invariant direction
    //    static const uint32 minNbrCells = 10;

    //static const uint32 defaultNbrPaddingCells = 0;

    GridLayoutImplInternals(uint32 nbDims, uint32 ghostParameter,
                            std::array<uint32,3> nbrCellsXYZ ,
                            std::array<double,3> dxdydz      );

    void initGridUtils( const gridDataT & data ) ;

    void initPhysicalStart( const gridDataT & data ) ;
    void initPhysicalEnd  ( const gridDataT & data ) ;
    void initGhostStart(    const gridDataT & data ) ;
    void initGhostEnd  (    const gridDataT & data ) ;

    QtyCentering changeCentering( QtyCentering layout ) const ;
    QtyCentering derivedCentering( HybridQuantity qty, Direction dir) const ;

    uint32 nbrPhysicalCells( Direction direction ) const noexcept;

    uint32 nbrGhosts( QtyCentering centering ) const noexcept;
    uint32 isDual( QtyCentering centering ) const noexcept;


    uint32 ghostCellIndexAtMax( QtyCentering centering, Direction direction ) const ;
};

#endif // GRIDLAYOUTIMPLINTERNALS_H
