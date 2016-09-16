#include <stdexcept>
#include <tuple>

#include "gridlayoutimplyee.h"




GridLayoutImplYee::GridLayoutImplYee(uint32 nbDims, uint32 interpOrder,
                                     std::array<uint32,3> nbrCellsXYZ ,
                                     std::array<double,3> dxdydz      )
    : GridLayoutImplInternals(nbDims, interpOrder,
                              nbrCellsXYZ, dxdydz)
{

    gridDataT gridData{} ;

    initGridUtils( gridData ) ;

    initLayoutCentering( gridData ) ;

    // all methods MUST BE CALLED AFTER initLayoutCentering()
    // because they USE data in hybridQtycentering_
    initPhysicalStart( gridData ) ;
    initPhysicalEnd  ( gridData ) ;
    initGhostStart( gridData ) ;
    initGhostEnd  ( gridData ) ;

}


void GridLayoutImplYee::initLayoutCentering( const gridDataT & data )
{

    hybridQtyCentering_[data.iBx ][data.idirX] = data.primal ;
    hybridQtyCentering_[data.iBx ][data.idirY] = data.dual   ;
    hybridQtyCentering_[data.iBx ][data.idirZ] = data.dual   ;
    hybridQtyCentering_[data.iBy ][data.idirX] = data.dual   ;
    hybridQtyCentering_[data.iBy ][data.idirY] = data.primal ;
    hybridQtyCentering_[data.iBy ][data.idirZ] = data.dual   ;
    hybridQtyCentering_[data.iBz ][data.idirX] = data.dual   ;
    hybridQtyCentering_[data.iBz ][data.idirY] = data.dual   ;
    hybridQtyCentering_[data.iBz ][data.idirZ] = data.primal ;
    hybridQtyCentering_[data.iEx ][data.idirX] = data.dual   ;
    hybridQtyCentering_[data.iEx ][data.idirY] = data.primal ;
    hybridQtyCentering_[data.iEx ][data.idirZ] = data.primal ;
    hybridQtyCentering_[data.iEy ][data.idirX] = data.primal ;
    hybridQtyCentering_[data.iEy ][data.idirY] = data.dual   ;
    hybridQtyCentering_[data.iEy ][data.idirZ] = data.primal ;
    hybridQtyCentering_[data.iEz ][data.idirX] = data.primal ;
    hybridQtyCentering_[data.iEz ][data.idirY] = data.primal ;
    hybridQtyCentering_[data.iEz ][data.idirZ] = data.dual   ;
    hybridQtyCentering_[data.irho][data.idirX] = data.primal ;
    hybridQtyCentering_[data.irho][data.idirY] = data.primal ;
    hybridQtyCentering_[data.irho][data.idirZ] = data.primal ;
    hybridQtyCentering_[data.iV  ][data.idirX] = data.primal ;
    hybridQtyCentering_[data.iV  ][data.idirY] = data.primal ;
    hybridQtyCentering_[data.iV  ][data.idirZ] = data.primal ;
    hybridQtyCentering_[data.iP  ][data.idirX] = data.primal ;
    hybridQtyCentering_[data.iP  ][data.idirY] = data.primal ;
    hybridQtyCentering_[data.iP  ][data.idirZ] = data.primal ;

}



AllocSizeT GridLayoutImplYee::allocSize( HybridQuantity qty ) const
{

    uint32 idirX = static_cast<uint32>( Direction::X ) ;
    uint32 idirY = static_cast<uint32>( Direction::Y ) ;
    uint32 idirZ = static_cast<uint32>( Direction::Z ) ;

    uint32 iQty = static_cast<uint32>(qty) ;

    std::array<QtyCentering, NBR_COMPO>
            qtyCenterings{ {hybridQtyCentering_[iQty][idirX],
                            hybridQtyCentering_[iQty][idirY],
                            hybridQtyCentering_[iQty][idirZ]} } ;

    uint32 nx =  2*nbrPaddingCells( Direction::X ) + nbrPhysicalCells( Direction::X ) + 1
               + nbrGhostAtMin( qtyCenterings[idirX] ) + nbrGhostAtMax( qtyCenterings[idirX] ) ;

    uint32 ny =  2*nbrPaddingCells( Direction::Y ) + nbrPhysicalCells( Direction::Y ) + 1
               + nbrGhostAtMin( qtyCenterings[idirY] ) + nbrGhostAtMax( qtyCenterings[idirY] ) ;

    uint32 nz =  2*nbrPaddingCells( Direction::Z ) + nbrPhysicalCells( Direction::Z ) + 1
               + nbrGhostAtMin( qtyCenterings[idirZ] ) + nbrGhostAtMax( qtyCenterings[idirZ] ) ;


    return AllocSizeT( nx, ny, nz );
}


// TODO : WARNING 1st order only
// Can it be moved to ImplInternals?
AllocSizeT  GridLayoutImplYee::allocSizeDerived( HybridQuantity qty, Direction dir ) const
{

    uint32 idirX = static_cast<uint32>( Direction::X ) ;
    uint32 idirY = static_cast<uint32>( Direction::Y ) ;
    uint32 idirZ = static_cast<uint32>( Direction::Z ) ;

    uint32 iDerivedDir = static_cast<uint32>( dir ) ;

    uint32 iQty = static_cast<uint32>(qty) ;

    std::array<QtyCentering, NBR_COMPO>
            qtyCenterings{ {hybridQtyCentering_[iQty][idirX],
                         hybridQtyCentering_[iQty][idirY],
                         hybridQtyCentering_[iQty][idirZ]} } ;

    QtyCentering newCentering = derivedCentering( qty, dir ) ;

    qtyCenterings[iDerivedDir] = newCentering ;

    uint32 nx =  2*nbrPaddingCells( Direction::X ) + nbrPhysicalCells( Direction::X ) + 1
               + nbrGhostAtMin( qtyCenterings[idirX] ) + nbrGhostAtMax( qtyCenterings[idirX] ) ;

    uint32 ny =  2*nbrPaddingCells( Direction::Y ) + nbrPhysicalCells( Direction::Y ) + 1
               + nbrGhostAtMin( qtyCenterings[idirY] ) + nbrGhostAtMax( qtyCenterings[idirY] ) ;

    uint32 nz =  2*nbrPaddingCells( Direction::Z ) + nbrPhysicalCells( Direction::Z ) + 1
               + nbrGhostAtMin( qtyCenterings[idirZ] ) + nbrGhostAtMax( qtyCenterings[idirZ] ) ;

    AllocSizeT allocSizes( nx, ny, nz ) ;

    return allocSizes ;
}



/**
 * @brief GridLayoutImplYee::fieldNodeCoordinates
 * this method should return a POINT, the idea is to make in every initializer
 * method, 3 nested loops over primal PhysicalStart/End indices
 * @param field
 * the returned point depends on the field's centering
 * @param origin
 * @param ix
 * @param iy
 * @param iz
 * @return Point
 * the desired physical coordinate
 */
Point GridLayoutImplYee::fieldNodeCoordinates(
        const Field & field, const Point & origin,
        uint32 ix, uint32 iy, uint32 iz ) const
{
    uint32 idirX = static_cast<uint32>(Direction::X) ;
    uint32 idirY = static_cast<uint32>(Direction::Y) ;
    uint32 idirZ = static_cast<uint32>(Direction::Z) ;

    uint32 ixStart = physicalStartIndex(field, Direction::X) ;
    uint32 iyStart = physicalStartIndex(field, Direction::Y) ;
    uint32 izStart = physicalStartIndex(field, Direction::Z) ;

    std::array<double, 3> halfCell{ {0, 0, 0} } ;

    uint32 iQty = static_cast<uint32>(field.hybridQty()) ;

    std::array<QtyCentering, 3> centering =
    { {hybridQtyCentering_[iQty][idirX],
       hybridQtyCentering_[iQty][idirY],
       hybridQtyCentering_[iQty][idirZ]} } ;

    for( uint32 idir=idirX ; idir<=idirZ ; ++idir)
    {
        if(centering[idir] == QtyCentering::dual) halfCell[idir] = 0.5 ;
    }

    // A shift of -dx/2, -dy/2, -dz/2 is necessary to get the physical
    // coordinate on the dual mesh
    // No shift for coordinate on the primal mesh

    double x = ( (ix-ixStart) - halfCell[0])*dx_ + origin.x_ ;
    double y = ( (iy-iyStart) - halfCell[1])*dy_ + origin.y_ ;
    double z = ( (iz-izStart) - halfCell[2])*dz_ + origin.z_ ;

    return Point(x, y, z) ;
}


/**
 * @brief GridLayoutImplYee::fieldNodeCoordinates
 * This method returns a cell-centered Point.
 * The idea is to call this method in every initializer method
 * using 3 nested loops over primal PhysicalStart/End indices.
 * @param origin
 * @param ix
 * @param iy
 * @param iz
 * @return Point
 * the desired cell-centered (dual/dual/dual) coordinate
 */
Point GridLayoutImplYee::cellCenteredCoordinates(
        const Point & origin,
        uint32 ix, uint32 iy, uint32 iz ) const
{

    uint32 ixStart = cellIndexAtMin( QtyCentering::primal, Direction::X ) ;
    uint32 iyStart = cellIndexAtMin( QtyCentering::primal, Direction::Y ) ;
    uint32 izStart = cellIndexAtMin( QtyCentering::primal, Direction::Z ) ;

    double halfCell = 0.5 ;
    // A shift of -dx/2, -dy/2, -dz/2 is necessary to get the
    // cell center physical coordinates,
    // because this point is located on the dual mesh

    double x = ( (ix-ixStart) - halfCell)*dx_ + origin.x_ ;
    double y = ( (iy-iyStart) - halfCell)*dy_ + origin.y_ ;
    double z = ( (iz-izStart) - halfCell)*dz_ + origin.z_ ;

    return Point(x, y, z) ;
}


void GridLayoutImplYee::deriv1D(Field const& operand, Field& derivative) const
{

    uint32 iOpStart = physicalStartIndex( operand, Direction::X ) ;
    uint32 iOpEnd   = physicalEndIndex  ( operand, Direction::X ) ;

    // The QtyCentering of derivative is given by
    // iQty = static_cast<uint32>( derivative.hybridQty() )
    // hybridQtyCentering_[iQty][idir]
    uint32 iDerStart = physicalStartIndex( derivative, Direction::X) ;

    uint32 iDirX = static_cast<uint32>( Direction::X ) ;

    uint32 iHybridQty = static_cast<uint32>( operand.hybridQty() ) ;

    QtyCentering opLayout = hybridQtyCentering_[iHybridQty][iDirX] ;


    uint32 iDer = 0 ;
    if( opLayout == QtyCentering::primal )
    {
        iDer = iDerStart + 1 ;
    } else  // opLayout on the dual
    {
        iDer = iDerStart ;
    }

    for( uint32 iOp=iOpStart ; iOp<iOpEnd ; ++iOp )
    {
        derivative(iDer) = odxdydz_[0] * ( operand(iOp+1) - operand(iOp) ) ;
        ++iDer ;
    }

}


// start and end index used in computing loops
uint32 GridLayoutImplYee::physicalStartIndex(Field const& field, Direction direction) const
{
    uint32 iQty = static_cast<uint32>(field.hybridQty());
    uint32 iDir = static_cast<uint32>(direction);
    return physicalStartIndex_[iQty][iDir];
}


uint32 GridLayoutImplYee::physicalEndIndex(Field const& field, Direction direction) const
{
    uint32 iQty = static_cast<uint32>(field.hybridQty());
    uint32 iDir = static_cast<uint32>(direction);
    return physicalEndIndex_[iQty][iDir];
}



uint32 GridLayoutImplYee::ghostStartIndex(Field const& field, Direction direction) const
{
    uint32 iQty = static_cast<uint32>(field.hybridQty());
    uint32 iDir = static_cast<uint32>(direction);
    return ghostStartIndex_[iQty][iDir];
}


uint32 GridLayoutImplYee::ghostEndIndex(Field const& field, Direction direction) const
{
    uint32 iQty = static_cast<uint32>(field.hybridQty());
    uint32 iDir = static_cast<uint32>(direction);
    return ghostEndIndex_[iQty][iDir];
}






