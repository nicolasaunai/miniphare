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
    return allocSize_(qty);
}





// TODO : WARNING 1st order only
// Can it be moved to ImplInternals?
AllocSizeT  GridLayoutImplYee::allocSizeDerived( HybridQuantity qty, Direction dir ) const
{
    return allocSizeDerived_(qty, dir);
}





// start and end index used in computing loops
uint32 GridLayoutImplYee::physicalStartIndex(Field const& field, Direction direction) const
{
    return physicalStartIndex_(field, direction);
}





uint32 GridLayoutImplYee::physicalStartIndex( QtyCentering centering,
                                              Direction direction     ) const
{
    return physicalStartIndex_(centering , direction); //cellIndexAtMin(centering, direction) ;
}





uint32 GridLayoutImplYee::physicalEndIndex(Field const& field, Direction direction) const
{
    return physicalEndIndex_(field, direction);
}





uint32 GridLayoutImplYee::physicalEndIndex( QtyCentering centering,
                                            Direction direction     ) const
{
    return physicalEndIndex_(centering, direction);//cellIndexAtMax(centering, direction) ;
}





uint32 GridLayoutImplYee::ghostStartIndex(Field const& field, Direction direction) const
{
    // should we directly return 0 and remove ghostStartIndex_ ?
    return ghostStartIndex_(field, direction);
}





uint32 GridLayoutImplYee::ghostEndIndex(Field const& field, Direction direction) const
{
    return ghostEndIndex_(field, direction);
}





Point GridLayoutImplYee::fieldNodeCoordinates(
        const Field & field, const Point & origin,
        uint32 ix, uint32 iy, uint32 iz ) const
{
    return fieldNodeCoordinates_( field, origin, ix, iy, iz ) ;
}






Point GridLayoutImplYee::cellCenteredCoordinates(
        const Point & origin, uint32 ix, uint32 iy, uint32 iz ) const
{
    return cellCenteredCoordinates_( origin, ix, iy, iz ) ;
}






void GridLayoutImplYee::deriv1D(Field const& operand, Field& derivative) const
{
    uint32 iDirX = static_cast<uint32>( Direction::X ) ;

    uint32 iQtyOperand = static_cast<uint32>( operand.hybridQty() ) ;

    QtyCentering opLayout = hybridQtyCentering_[iQtyOperand][iDirX] ;


    // The QtyCentering of derivative is given by
    // iQty = static_cast<uint32>( derivative.hybridQty() )
    // hybridQtyCentering_[iQty][idir]
    uint32 iDerStart = physicalStartIndex( derivative, Direction::X) ;

    uint32 iDer = iDerStart ;
    if( opLayout == QtyCentering::dual )
    {
        iDer++ ;
    }

    uint32 iOpStart = physicalStartIndex( operand, Direction::X ) ;
    uint32 iOpEnd   = physicalEndIndex  ( operand, Direction::X ) ;

    for( uint32 iOp=iOpStart ; iOp<iOpEnd ; ++iOp )
    {
        derivative(iDer) = odxdydz_[0] * ( operand(iOp+1) - operand(iOp) ) ;
        ++iDer ;
    }

}









