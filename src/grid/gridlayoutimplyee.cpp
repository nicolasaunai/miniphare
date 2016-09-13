#include <stdexcept>
#include <tuple>

#include "gridlayoutimplyee.h"




GridLayoutImplYee::GridLayoutImplYee(uint32 nbDims, uint32 interpOrder,
                                     std::array<uint32,3> nbrCellsXYZ ,
                                     std::array<double,3> dxdydz      )
    : GridLayoutImplInternals(nbDims, interpOrder,
                              nbrCellsXYZ, dxdydz)
{
    Direction dirX = Direction::directionX ;
    Direction dirY = Direction::directionY ;
    Direction dirZ = Direction::directionZ ;

    LayoutType primal = LayoutType::primal ;
    LayoutType dual   = LayoutType::dual   ;

    uint32 idirX = static_cast<uint32>(Direction::directionX) ;
    uint32 idirY = static_cast<uint32>(Direction::directionY) ;
    uint32 idirZ = static_cast<uint32>(Direction::directionZ) ;

    uint32 iBx = static_cast<uint32>(HybridQuantity::Bx) ;
    uint32 iBy = static_cast<uint32>(HybridQuantity::By) ;
    uint32 iBz = static_cast<uint32>(HybridQuantity::Bz) ;

    uint32 iEx = static_cast<uint32>(HybridQuantity::Ex) ;
    uint32 iEy = static_cast<uint32>(HybridQuantity::Ey) ;
    uint32 iEz = static_cast<uint32>(HybridQuantity::Ez) ;

    uint32 irho = static_cast<uint32>(HybridQuantity::rho) ;
    uint32 iV = static_cast<uint32>(HybridQuantity::V) ;
    uint32 iP = static_cast<uint32>(HybridQuantity::P) ;


    hybridQtyCentering_[iBx][idirX]  = primal ;
    hybridQtyCentering_[iBx][idirY]  = dual   ;
    hybridQtyCentering_[iBx][idirZ]  = dual   ;

    hybridQtyCentering_[iBy][idirX]  = dual   ;
    hybridQtyCentering_[iBy][idirY]  = primal ;
    hybridQtyCentering_[iBy][idirZ]  = dual   ;

    hybridQtyCentering_[iBz][idirX]  = dual   ;
    hybridQtyCentering_[iBz][idirY]  = dual   ;
    hybridQtyCentering_[iBz][idirZ]  = primal ;

    hybridQtyCentering_[iEx][idirX]  = dual   ;
    hybridQtyCentering_[iEx][idirY]  = primal ;
    hybridQtyCentering_[iEx][idirZ]  = primal ;

    hybridQtyCentering_[iEy][idirX]  = primal ;
    hybridQtyCentering_[iEy][idirY]  = dual   ;
    hybridQtyCentering_[iEy][idirZ]  = primal ;

    hybridQtyCentering_[iEz][idirX]  = primal ;
    hybridQtyCentering_[iEz][idirY]  = primal ;
    hybridQtyCentering_[iEz][idirZ]  = dual   ;

    hybridQtyCentering_[irho][idirX] = primal ;
    hybridQtyCentering_[irho][idirY] = primal ;
    hybridQtyCentering_[irho][idirZ] = primal ;

    hybridQtyCentering_[iV][idirX]   = primal ;
    hybridQtyCentering_[iV][idirY]   = primal ;
    hybridQtyCentering_[iV][idirZ]   = primal ;

    hybridQtyCentering_[iP][idirX]   = primal ;
    hybridQtyCentering_[iP][idirY]   = primal ;
    hybridQtyCentering_[iP][idirZ]   = primal ;


    physicalStartIndex_[iBx][idirX]  = cellIndexAtMin( hybridQtyCentering_[iBx][idirX], dirX ) ;
    physicalStartIndex_[iBx][idirY]  = cellIndexAtMin( hybridQtyCentering_[iBx][idirY], dirY ) ;
    physicalStartIndex_[iBx][idirZ]  = cellIndexAtMin( hybridQtyCentering_[iBx][idirZ], dirZ ) ;

    physicalStartIndex_[iBy][idirX]  = cellIndexAtMin( hybridQtyCentering_[iBy][idirX], dirX ) ;
    physicalStartIndex_[iBy][idirY]  = cellIndexAtMin( hybridQtyCentering_[iBy][idirY], dirY ) ;
    physicalStartIndex_[iBy][idirZ]  = cellIndexAtMin( hybridQtyCentering_[iBy][idirZ], dirZ ) ;

    physicalStartIndex_[iBz][idirX]  = cellIndexAtMin( hybridQtyCentering_[iBz][idirX], dirX ) ;
    physicalStartIndex_[iBz][idirY]  = cellIndexAtMin( hybridQtyCentering_[iBz][idirY], dirY ) ;
    physicalStartIndex_[iBz][idirZ]  = cellIndexAtMin( hybridQtyCentering_[iBz][idirZ], dirZ ) ;

    physicalStartIndex_[iEx][idirX]  = cellIndexAtMin( hybridQtyCentering_[iEx][idirX], dirX ) ;
    physicalStartIndex_[iEy][idirX]  = cellIndexAtMin( hybridQtyCentering_[iEy][idirX], dirX ) ;
    physicalStartIndex_[iEz][idirX]  = cellIndexAtMin( hybridQtyCentering_[iEz][idirX], dirX ) ;

    physicalStartIndex_[iEx][idirY]  = cellIndexAtMin( hybridQtyCentering_[iEx][idirY], dirY ) ;
    physicalStartIndex_[iEy][idirY]  = cellIndexAtMin( hybridQtyCentering_[iEy][idirY], dirY ) ;
    physicalStartIndex_[iEz][idirY]  = cellIndexAtMin( hybridQtyCentering_[iEz][idirY], dirY ) ;

    physicalStartIndex_[iEx][idirZ]  = cellIndexAtMin( hybridQtyCentering_[iEx][idirZ], dirZ ) ;
    physicalStartIndex_[iEy][idirZ]  = cellIndexAtMin( hybridQtyCentering_[iEy][idirZ], dirZ ) ;
    physicalStartIndex_[iEz][idirZ]  = cellIndexAtMin( hybridQtyCentering_[iEz][idirZ], dirZ ) ;

    physicalStartIndex_[irho][idirX] = cellIndexAtMin( hybridQtyCentering_[irho][idirX], dirX ) ;
    physicalStartIndex_[irho][idirY] = cellIndexAtMin( hybridQtyCentering_[irho][idirY], dirY ) ;
    physicalStartIndex_[irho][idirZ] = cellIndexAtMin( hybridQtyCentering_[irho][idirZ], dirZ ) ;

    physicalStartIndex_[iV  ][idirX] = cellIndexAtMin( hybridQtyCentering_[iV  ][idirX], dirX ) ;
    physicalStartIndex_[iV  ][idirY] = cellIndexAtMin( hybridQtyCentering_[iV  ][idirY], dirY ) ;
    physicalStartIndex_[iV  ][idirZ] = cellIndexAtMin( hybridQtyCentering_[iV  ][idirZ], dirZ ) ;

    physicalStartIndex_[iP  ][idirX] = cellIndexAtMin( hybridQtyCentering_[iP  ][idirX], dirX ) ;
    physicalStartIndex_[iP  ][idirY] = cellIndexAtMin( hybridQtyCentering_[iP  ][idirY], dirY ) ;
    physicalStartIndex_[iP  ][idirZ] = cellIndexAtMin( hybridQtyCentering_[iP  ][idirZ], dirZ ) ;


    physicalEndIndex_[iBx][idirX]  = cellIndexAtMax( hybridQtyCentering_[iBx][idirX], dirX ) ;
    physicalEndIndex_[iBx][idirY]  = cellIndexAtMax( hybridQtyCentering_[iBx][idirY], dirY ) ;
    physicalEndIndex_[iBx][idirZ]  = cellIndexAtMax( hybridQtyCentering_[iBx][idirZ], dirZ ) ;

    physicalEndIndex_[iBy][idirX]  = cellIndexAtMax( hybridQtyCentering_[iBy][idirX], dirX ) ;
    physicalEndIndex_[iBy][idirY]  = cellIndexAtMax( hybridQtyCentering_[iBy][idirY], dirY ) ;
    physicalEndIndex_[iBy][idirZ]  = cellIndexAtMax( hybridQtyCentering_[iBy][idirZ], dirZ ) ;

    physicalEndIndex_[iBz][idirX]  = cellIndexAtMax( hybridQtyCentering_[iBz][idirX], dirX ) ;
    physicalEndIndex_[iBz][idirY]  = cellIndexAtMax( hybridQtyCentering_[iBz][idirY], dirY ) ;
    physicalEndIndex_[iBz][idirZ]  = cellIndexAtMax( hybridQtyCentering_[iBz][idirZ], dirZ ) ;

    physicalEndIndex_[iEx][idirX]  = cellIndexAtMax( hybridQtyCentering_[iEx][idirX], dirX ) ;
    physicalEndIndex_[iEx][idirY]  = cellIndexAtMax( hybridQtyCentering_[iEx][idirY], dirY ) ;
    physicalEndIndex_[iEx][idirZ]  = cellIndexAtMax( hybridQtyCentering_[iEx][idirZ], dirZ ) ;

    physicalEndIndex_[iEy][idirX]  = cellIndexAtMax( hybridQtyCentering_[iEy][idirX], dirX ) ;
    physicalEndIndex_[iEy][idirY]  = cellIndexAtMax( hybridQtyCentering_[iEy][idirY], dirY ) ;
    physicalEndIndex_[iEy][idirZ]  = cellIndexAtMax( hybridQtyCentering_[iEy][idirZ], dirZ ) ;

    physicalEndIndex_[iEz][idirX]  = cellIndexAtMax( hybridQtyCentering_[iEz][idirX], dirX ) ;
    physicalEndIndex_[iEz][idirY]  = cellIndexAtMax( hybridQtyCentering_[iEz][idirY], dirY ) ;
    physicalEndIndex_[iEz][idirZ]  = cellIndexAtMax( hybridQtyCentering_[iEz][idirZ], dirZ ) ;

    physicalEndIndex_[irho][idirX] = cellIndexAtMax( hybridQtyCentering_[irho][idirX], dirX ) ;
    physicalEndIndex_[irho][idirY] = cellIndexAtMax( hybridQtyCentering_[irho][idirY], dirY ) ;
    physicalEndIndex_[irho][idirZ] = cellIndexAtMax( hybridQtyCentering_[irho][idirZ], dirZ ) ;

    physicalEndIndex_[iV  ][idirX] = cellIndexAtMax( hybridQtyCentering_[iV  ][idirX], dirX ) ;
    physicalEndIndex_[iV  ][idirY] = cellIndexAtMax( hybridQtyCentering_[iV  ][idirY], dirY ) ;
    physicalEndIndex_[iV  ][idirZ] = cellIndexAtMax( hybridQtyCentering_[iV  ][idirZ], dirZ ) ;

    physicalEndIndex_[iP  ][idirX] = cellIndexAtMax( hybridQtyCentering_[iP  ][idirX], dirX ) ;
    physicalEndIndex_[iP  ][idirY] = cellIndexAtMax( hybridQtyCentering_[iP  ][idirY], dirY ) ;
    physicalEndIndex_[iP  ][idirZ] = cellIndexAtMax( hybridQtyCentering_[iP  ][idirZ], dirZ ) ;


    ghostStartIndex_[iBx][idirX]  = nbrPaddingCells( dirX ) ;
    ghostStartIndex_[iBx][idirY]  = nbrPaddingCells( dirY ) ;
    ghostStartIndex_[iBx][idirZ]  = nbrPaddingCells( dirZ ) ;

    ghostStartIndex_[iBy][idirX]  = nbrPaddingCells( dirX ) ;
    ghostStartIndex_[iBy][idirY]  = nbrPaddingCells( dirY ) ;
    ghostStartIndex_[iBy][idirZ]  = nbrPaddingCells( dirZ ) ;

    ghostStartIndex_[iBz][idirX]  = nbrPaddingCells( dirX ) ;
    ghostStartIndex_[iBz][idirY]  = nbrPaddingCells( dirY ) ;
    ghostStartIndex_[iBz][idirZ]  = nbrPaddingCells( dirZ ) ;

    ghostStartIndex_[iEx][idirX]  = nbrPaddingCells( dirX ) ;
    ghostStartIndex_[iEx][idirY]  = nbrPaddingCells( dirY ) ;
    ghostStartIndex_[iEx][idirZ]  = nbrPaddingCells( dirZ ) ;

    ghostStartIndex_[iEy][idirX]  = nbrPaddingCells( dirX ) ;
    ghostStartIndex_[iEy][idirY]  = nbrPaddingCells( dirY ) ;
    ghostStartIndex_[iEy][idirZ]  = nbrPaddingCells( dirZ ) ;

    ghostStartIndex_[iEz][idirX]  = nbrPaddingCells( dirX ) ;
    ghostStartIndex_[iEz][idirY]  = nbrPaddingCells( dirY ) ;
    ghostStartIndex_[iEz][idirZ]  = nbrPaddingCells( dirZ ) ;

    ghostStartIndex_[irho][idirX] = nbrPaddingCells( dirX ) ;
    ghostStartIndex_[irho][idirY] = nbrPaddingCells( dirY ) ;
    ghostStartIndex_[irho][idirZ] = nbrPaddingCells( dirZ ) ;

    ghostStartIndex_[iV  ][idirX] = nbrPaddingCells( dirX ) ;
    ghostStartIndex_[iV  ][idirY] = nbrPaddingCells( dirY ) ;
    ghostStartIndex_[iV  ][idirZ] = nbrPaddingCells( dirZ ) ;

    ghostStartIndex_[iP  ][idirX] = nbrPaddingCells( dirX ) ;
    ghostStartIndex_[iP  ][idirY] = nbrPaddingCells( dirY ) ;
    ghostStartIndex_[iP  ][idirZ] = nbrPaddingCells( dirZ ) ;


    ghostEndIndex_[iBx][idirX]  = ghostCellIndexAtMax( hybridQtyCentering_[iBx][idirX], dirX ) ;
    ghostEndIndex_[iBx][idirY]  = ghostCellIndexAtMax( hybridQtyCentering_[iBx][idirY], dirY ) ;
    ghostEndIndex_[iBx][idirZ]  = ghostCellIndexAtMax( hybridQtyCentering_[iBx][idirZ], dirZ ) ;

    ghostEndIndex_[iBy][idirX]  = ghostCellIndexAtMax( hybridQtyCentering_[iBy][idirX], dirX ) ;
    ghostEndIndex_[iBy][idirY]  = ghostCellIndexAtMax( hybridQtyCentering_[iBy][idirY], dirY ) ;
    ghostEndIndex_[iBy][idirZ]  = ghostCellIndexAtMax( hybridQtyCentering_[iBy][idirZ], dirZ ) ;

    ghostEndIndex_[iBz][idirX]  = ghostCellIndexAtMax( hybridQtyCentering_[iBz][idirX], dirX ) ;
    ghostEndIndex_[iBz][idirY]  = ghostCellIndexAtMax( hybridQtyCentering_[iBz][idirY], dirY ) ;
    ghostEndIndex_[iBz][idirZ]  = ghostCellIndexAtMax( hybridQtyCentering_[iBz][idirZ], dirZ ) ;

    ghostEndIndex_[iEx][idirX]  = ghostCellIndexAtMax( hybridQtyCentering_[iEx][idirX], dirX ) ;
    ghostEndIndex_[iEx][idirY]  = ghostCellIndexAtMax( hybridQtyCentering_[iEx][idirY], dirY ) ;
    ghostEndIndex_[iEx][idirZ]  = ghostCellIndexAtMax( hybridQtyCentering_[iEx][idirZ], dirZ ) ;

    ghostEndIndex_[iEy][idirX]  = ghostCellIndexAtMax( hybridQtyCentering_[iEy][idirX], dirX ) ;
    ghostEndIndex_[iEy][idirY]  = ghostCellIndexAtMax( hybridQtyCentering_[iEy][idirY], dirY ) ;
    ghostEndIndex_[iEy][idirZ]  = ghostCellIndexAtMax( hybridQtyCentering_[iEy][idirZ], dirZ ) ;

    ghostEndIndex_[iEz][idirX]  = ghostCellIndexAtMax( hybridQtyCentering_[iEz][idirX], dirX ) ;
    ghostEndIndex_[iEz][idirY]  = ghostCellIndexAtMax( hybridQtyCentering_[iEz][idirY], dirY ) ;
    ghostEndIndex_[iEz][idirZ]  = ghostCellIndexAtMax( hybridQtyCentering_[iEz][idirZ], dirZ ) ;

    ghostEndIndex_[irho][idirX] = ghostCellIndexAtMax( hybridQtyCentering_[irho][idirX], dirX ) ;
    ghostEndIndex_[irho][idirY] = ghostCellIndexAtMax( hybridQtyCentering_[irho][idirY], dirY ) ;
    ghostEndIndex_[irho][idirZ] = ghostCellIndexAtMax( hybridQtyCentering_[irho][idirZ], dirZ ) ;

    ghostEndIndex_[iV  ][idirX] = ghostCellIndexAtMax( hybridQtyCentering_[iV  ][idirX], dirX ) ;
    ghostEndIndex_[iV  ][idirY] = ghostCellIndexAtMax( hybridQtyCentering_[iV  ][idirY], dirY ) ;
    ghostEndIndex_[iV  ][idirZ] = ghostCellIndexAtMax( hybridQtyCentering_[iV  ][idirZ], dirZ ) ;

    ghostEndIndex_[iP  ][idirX] = ghostCellIndexAtMax( hybridQtyCentering_[iP  ][idirX], dirX ) ;
    ghostEndIndex_[iP  ][idirY] = ghostCellIndexAtMax( hybridQtyCentering_[iP  ][idirY], dirY ) ;
    ghostEndIndex_[iP  ][idirZ] = ghostCellIndexAtMax( hybridQtyCentering_[iP  ][idirZ], dirZ ) ;

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

    uint32 nx =  2*nbrPaddingCells( dirX ) + nbrPhysicalCells( dirX )
               + nbrGhostAtMin( qtyLayouts[idirX] ) + nbrGhostAtMax( qtyLayouts[idirX] ) ;

    uint32 ny =  2*nbrPaddingCells( dirY ) + nbrPhysicalCells( dirY )
               + nbrGhostAtMin( qtyLayouts[idirY] ) + nbrGhostAtMax( qtyLayouts[idirY] ) ;

    uint32 nz =  2*nbrPaddingCells( dirZ ) + nbrPhysicalCells( dirZ )
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

    uint32 iFx = static_cast<uint32>(field.type()) ;

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


void GridLayoutImplYee::deriv(Field const& operand, Direction direction, Field& derivative) const
{

    uint32 iStart = physicalStartIndex( operand, direction ) ;

//    for( uint32 ik=physicalStartIndex())
//    {

//    }


}


// start and end index used in computing loops
uint32 GridLayoutImplYee::physicalStartIndex(Field const& field, Direction direction) const
{

    return physicalStartIndex_[static_cast<uint32>(field.type())][static_cast<uint32>(direction)];
}


uint32 GridLayoutImplYee::physicalEndIndex(Field const& field, Direction direction) const
{

    return physicalEndIndex_[static_cast<uint32>(field.type())][static_cast<uint32>(direction)];
}



uint32 GridLayoutImplYee::ghostStartIndex(Field const& field, Direction direction) const
{

    return ghostStartIndex_[static_cast<uint32>(field.type())][static_cast<uint32>(direction)];
}


uint32 GridLayoutImplYee::ghostEndIndex(Field const& field, Direction direction) const
{

    return ghostEndIndex_[static_cast<uint32>(field.type())][static_cast<uint32>(direction)];
}






