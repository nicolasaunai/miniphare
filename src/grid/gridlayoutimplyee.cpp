#include <stdexcept>
#include <tuple>

#include "gridlayoutimplyee.h"




GridLayoutImplYee::GridLayoutImplYee(uint32 nbDims, uint32 interpOrder,
                                     std::array<uint32,3> nbrCellsXYZ ,
                                     std::array<double,3> dxdydz      )
    : GridLayoutImplInternals(nbDims, interpOrder,
                              nbrCellsXYZ, dxdydz)
{

    gridDataT staticData{} ;

    initGridUtils( staticData ) ;

    initLayoutCentering( staticData ) ;

    initPhysicalStart( staticData ) ;
    initPhysicalEnd  ( staticData ) ;
    initGhostStart( staticData ) ;
    initGhostEnd  ( staticData ) ;

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



std::array<AllocSizeT, NBR_COMPO> GridLayoutImplYee::allocSize( EMFieldType fieldType ) const
{    
    using Alloc = AllocSizeT ;

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

    switch( fieldType )
    {
    case EMFieldType::EVecField:
        iFx = static_cast<uint32>(HybridQuantity::Ex) ;
        iFy = static_cast<uint32>(HybridQuantity::Ey) ;
        iFz = static_cast<uint32>(HybridQuantity::Ez) ;
        break ;

    case EMFieldType::BVecField:
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


    std::array<Alloc, NBR_COMPO> VecFieldSizes{
                   {Alloc(VFx_nx, VFx_ny, VFx_nz),
                    Alloc(VFy_nx, VFy_ny, VFy_nz),
                    Alloc(VFz_nx, VFz_ny, VFz_nz)} } ;

    return VecFieldSizes ;
}


AllocSizeT  GridLayoutImplYee::allocSizeDerived( HybridQuantity qty, Direction dir ) const
{
    Direction dirX = Direction::directionX ;
    Direction dirY = Direction::directionY ;
    Direction dirZ = Direction::directionZ ;

    uint32 idirX = static_cast<uint32>( dirX ) ;
    uint32 idirY = static_cast<uint32>( dirY ) ;
    uint32 idirZ = static_cast<uint32>( dirZ ) ;

    uint32 iDerivedDir = static_cast<uint32>( dir ) ;

    uint32 iQty = static_cast<uint32>(qty) ;

    std::array<LayoutType, NBR_COMPO>
            qtyLayouts{ {hybridQtyCentering_[iQty][idirX],
                         hybridQtyCentering_[iQty][idirY],
                         hybridQtyCentering_[iQty][idirZ]} } ;

    LayoutType layout = derivedLayout( qty, dir ) ;

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


std::array<AllocSizeT, NBR_COMPO> GridLayoutImplYee::allocSize( OhmTerm term ) const
{
}


std::vector< std::tuple <uint32, Point> >
GridLayoutImplYee::fieldNodeCoordinates1D(
        const Field & field, const Point & patchOrigin ) const
{
    Direction dirX = Direction::directionX ;

    uint32 idirX = static_cast<uint32>(dirX) ;

    std::vector< std::tuple<uint32, Point> > nodeList ;

    double half_cell = 0. ;

    uint32 iFx = static_cast<uint32>(field.hybridQty()) ;

    LayoutType centering = hybridQtyCentering_[iFx][idirX] ;

    if( centering == LayoutType::dual )
    {
        half_cell = 0.5 ;
    }


    double y = patchOrigin.y_ ;
    double z = patchOrigin.z_ ;

    uint32 ixStart = physicalStartIndex(field, dirX) ;
    uint32 ixEnd   = physicalEndIndex  (field, dirX) ;

    for( uint32 ix= ixStart; ix< ixEnd ; ++ix )
    {
        double x = ( (ix-ixStart) + half_cell)*dx_ + patchOrigin.x_ ;

        PointT point( x, y, z ) ;

        auto nodeCoordinates = std::make_tuple(ix, point) ;

        nodeList.push_back( nodeCoordinates ) ;
    }

    return nodeList ;
}


void GridLayoutImplYee::deriv1D(Field const& operand, Direction direction, Field& derivative) const
{

    uint32 iOpStart = physicalStartIndex( operand, direction ) ;
    uint32 iOpEnd   = physicalEndIndex  ( operand, direction ) ;

    uint32 iDerStart = physicalStartIndex( derivative, direction ) ;
//    uint32 iDerEnd   = physicalEndIndex  ( derivative, direction ) ;

    uint32 iDirX = static_cast<uint32>( Direction::directionX ) ;

    uint32 iHybridQty = static_cast<uint32>( operand.hybridQty() ) ;

    LayoutType opLayout = hybridQtyCentering_[iHybridQty][iDirX] ;

    double ods = inverseSpatialStep( direction ) ;

    uint32 iDer = 0 ;
    if( opLayout == LayoutType::primal )
    {
        iDer = iDerStart + 1 ;
    } else  // opLayout on the dual
    {
        iDer = iDerStart ;
    }

    for( uint32 iOp=iOpStart ; iOp<iOpEnd ; ++iOp )
    {
        derivative(iDer) = ods * ( operand(iOp+1) - operand(iOp) ) ;
        ++iDer ;
    }

}


// start and end index used in computing loops
uint32 GridLayoutImplYee::physicalStartIndex(Field const& field, Direction direction) const
{

    return physicalStartIndex_[static_cast<uint32>(field.hybridQty())][static_cast<uint32>(direction)];
}


uint32 GridLayoutImplYee::physicalEndIndex(Field const& field, Direction direction) const
{

    return physicalEndIndex_[static_cast<uint32>(field.hybridQty())][static_cast<uint32>(direction)];
}



uint32 GridLayoutImplYee::ghostStartIndex(Field const& field, Direction direction) const
{

    return ghostStartIndex_[static_cast<uint32>(field.hybridQty())][static_cast<uint32>(direction)];
}


uint32 GridLayoutImplYee::ghostEndIndex(Field const& field, Direction direction) const
{

    return ghostEndIndex_[static_cast<uint32>(field.hybridQty())][static_cast<uint32>(direction)];
}






