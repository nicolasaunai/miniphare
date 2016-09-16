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


/* TODO change this so that all arrays (including ions and species moments) can
   be allocated.


   think whether the method (which only depends on hybridQtyCentering)
   can be moved into GridLayoutImplInternals


Species::Species(GridLayout const& layout, double mass,
                 ParticleInitializer const& particleInitializer,
                 std::string const& name)
    : layout_{ layout },
      rho_    { layout_.nx(), layout_.ny(), layout_.nz(), "rho_"   + name },
      bulkVel_{ layout_.nx(), layout_.ny(), layout_.nz(),"bulkVel_"+ name },
      particleArray_{},
      particleInitializer_{ particleInitializer.clone() }
{

}


   hint : use HybridQtyCentering.

*/
AllocSizeT GridLayoutImplYee::allocSize( HybridQuantity qtyType ) const
{


#if 0
    Direction dirX = Direction::directionX ;
    Direction dirY = Direction::directionY ;
    Direction dirZ = Direction::directionZ ;

    uint32 idirX = static_cast<uint32>(Direction::directionX) ;
    uint32 idirY = static_cast<uint32>(Direction::directionY) ;
    uint32 idirZ = static_cast<uint32>(Direction::directionZ) ;

    uint32 iFx, iFy, iFz ;

    uint32 VFx_nx, VFx_ny, VFx_nz ;
    uint32 VFy_nx, VFy_ny, VFy_nz ;
    uint32 VFz_nx, VFz_ny, VFz_nz ;

    switch( qtyType )
    {
    case HybridQuantity::Ex :
        iFx = static_cast<uint32>(HybridQuantity::Ex) ;
        iFy = static_cast<uint32>(HybridQuantity::Ey) ;
        iFz = static_cast<uint32>(HybridQuantity::Ez) ;
        break ;

    case  HybridQuantity::Bx:
        iFx = static_cast<uint32>(HybridQuantity::Bx) ;
        iFy = static_cast<uint32>(HybridQuantity::By) ;
        iFz = static_cast<uint32>(HybridQuantity::Bz) ;
        break ;
    }

    VFx_nx = ghostEndIndex_[iFx][idirX] - ghostStartIndex_[iFx][idirX] + 1
            + 2*nbrPaddingCells(dirX) ;
    VFx_ny = ghostEndIndex_[iFx][idirY] - ghostStartIndex_[iFx][idirY] + 1
            + 2*nbrPaddingCells(dirY) ;
    VFx_nz = ghostEndIndex_[iFx][idirZ] - ghostStartIndex_[iFx][idirZ] + 1
            + 2*nbrPaddingCells(dirZ) ;
    VFy_nx = ghostEndIndex_[iFy][idirX] - ghostStartIndex_[iFy][idirX] + 1
            + 2*nbrPaddingCells(dirX) ;
    VFy_ny = ghostEndIndex_[iFy][idirY] - ghostStartIndex_[iFy][idirY] + 1
            + 2*nbrPaddingCells(dirY) ;
    VFy_nz = ghostEndIndex_[iFy][idirZ] - ghostStartIndex_[iFy][idirZ] + 1
            + 2*nbrPaddingCells(dirZ) ;
    VFz_nx = ghostEndIndex_[iFz][idirX] - ghostStartIndex_[iFz][idirX] + 1
            + 2*nbrPaddingCells(dirX) ;
    VFz_ny = ghostEndIndex_[iFz][idirY] - ghostStartIndex_[iFz][idirY] + 1
            + 2*nbrPaddingCells(dirY) ;
    VFz_nz = ghostEndIndex_[iFz][idirZ] - ghostStartIndex_[iFz][idirZ] + 1
            + 2*nbrPaddingCells(dirZ) ;
return AllocSizeT(VFx_nx, VFx_ny, VFx_nz);
#endif
    return AllocSizeT(0,0,0);

}

// TODO attention 1st order only. and can it be moved to ImplInternals?
AllocSizeT  GridLayoutImplYee::allocSizeDerived( HybridQuantity qty, Direction dir ) const
{
    Direction dirX = Direction::X ;
    Direction dirY = Direction::Y ;
    Direction dirZ = Direction::Z ;

    uint32 idirX = static_cast<uint32>( dirX ) ;
    uint32 idirY = static_cast<uint32>( dirY ) ;
    uint32 idirZ = static_cast<uint32>( dirZ ) ;

    uint32 iDerivedDir = static_cast<uint32>( dir ) ;

    uint32 iQty = static_cast<uint32>(qty) ;

    std::array<QtyCentering, NBR_COMPO>
            qtyLayouts{ {hybridQtyCentering_[iQty][idirX],
                         hybridQtyCentering_[iQty][idirY],
                         hybridQtyCentering_[iQty][idirZ]} } ;

    QtyCentering layout = derivedLayout( qty, dir ) ;

    qtyLayouts[iDerivedDir] = layout ;

    uint32 nx =  2*nbrPaddingCells( dirX ) + nbrPhysicalCells( dirX ) + 1
               + nbrGhostAtMin( qtyLayouts[idirX] ) + nbrGhostAtMax( qtyLayouts[idirX] ) ;

    uint32 ny =  2*nbrPaddingCells( dirY ) + nbrPhysicalCells( dirY ) + 1
               + nbrGhostAtMin( qtyLayouts[idirY] ) + nbrGhostAtMax( qtyLayouts[idirY] ) ;

    uint32 nz =  2*nbrPaddingCells( dirZ ) + nbrPhysicalCells( dirZ ) + 1
               + nbrGhostAtMin( qtyLayouts[idirZ] ) + nbrGhostAtMax( qtyLayouts[idirZ] ) ;

    AllocSizeT allocSizes( nx, ny, nz ) ;

    return allocSizes ;
}



/**
 * @brief GridLayoutImplYee::fieldNodeCoordinates
 * this method should return a POINT, the idea is to make in every initializer
 * method, 3 nested loops over PhysicalStart/End indices
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

    double x = ( (ix-ixStart) - halfCell[0])*dx_ + origin.x_ ;
    double y = ( (iy-iyStart) - halfCell[1])*dy_ + origin.y_ ;
    double z = ( (iz-izStart) - halfCell[2])*dz_ + origin.z_ ;

    return Point(x, y, z) ;
}




void GridLayoutImplYee::deriv1D(Field const& operand, Field& derivative) const
{

    uint32 iOpStart = physicalStartIndex( operand, Direction::X ) ;
    uint32 iOpEnd   = physicalEndIndex  ( operand, Direction::X ) ;

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






