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
 * by GridLayoutImplInternals' methods. A lot of operations on a quantity
 * actually only depend on the centering of the quantity, namely whether it is
 * a primal or dual centering. The only thing GridlayoutImplInternals
 *
 */
class GridLayoutImplInternals
{

    // ------------------------------------------------------------------------
    //                              PRIVATE
    //
    //   this is just for GridLayoutImplInternals
    // ------------------------------------------------------------------------
private:

    std::array<uint32, NBR_COMPO>
    nodeNbrFromCentering_( std::array<QtyCentering, NBR_COMPO> const & qtyCenterings ) const;



    // ------------------------------------------------------------------------
    //                             PROTECTED
    //
    // this code will be shared by all concreteof GridLayoutImpl*
    // ------------------------------------------------------------------------
protected:

    uint32 nbdims_;

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


    GridLayoutImplInternals(uint32 nbDims, uint32 ghostParameter,
                            std::array<uint32,3> nbrCellsXYZ ,
                            std::array<double,3> dxdydz      );


    double inverseSpatialStep( Direction direction ) const noexcept
    {return odxdydz_[static_cast<uint32>(direction)]; }


    void computeNbrGhosts(uint32 ghostParameter ) ;


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



    void initPhysicalStart( const gridDataT & data ) ;
    void initPhysicalEnd  ( const gridDataT & data ) ;
    void initGhostStart(    const gridDataT & data ) ;
    void initGhostEnd  (    const gridDataT & data ) ;

    QtyCentering changeCentering( QtyCentering layout ) const ;
    QtyCentering derivedCentering( HybridQuantity qty, Direction dir) const ;


    uint32 nbrGhosts( QtyCentering centering ) const noexcept;
    uint32 isDual( QtyCentering centering ) const noexcept;


    uint32 ghostCellIndexAtMax( QtyCentering centering, Direction direction ) const ;
};

#endif // GRIDLAYOUTIMPLINTERNALS_H
