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


    physicalStartIndex_[iBx][idirX]  = cellNbrMin( hybridQtyCentering_[iBx][idirX], dirX ) ;
    physicalStartIndex_[iBx][idirY]  = cellNbrMin( hybridQtyCentering_[iBx][idirY], dirY ) ;
    physicalStartIndex_[iBx][idirZ]  = cellNbrMin( hybridQtyCentering_[iBx][idirZ], dirZ ) ;

    physicalStartIndex_[iBy][idirX]  = cellNbrMin( hybridQtyCentering_[iBy][idirX], dirX ) ;
    physicalStartIndex_[iBy][idirY]  = cellNbrMin( hybridQtyCentering_[iBy][idirY], dirY ) ;
    physicalStartIndex_[iBy][idirZ]  = cellNbrMin( hybridQtyCentering_[iBy][idirZ], dirZ ) ;

    physicalStartIndex_[iBz][idirX]  = cellNbrMin( hybridQtyCentering_[iBz][idirX], dirX ) ;
    physicalStartIndex_[iBz][idirY]  = cellNbrMin( hybridQtyCentering_[iBz][idirY], dirY ) ;
    physicalStartIndex_[iBz][idirZ]  = cellNbrMin( hybridQtyCentering_[iBz][idirZ], dirZ ) ;

    physicalStartIndex_[iEx][idirX]  = cellNbrMin( hybridQtyCentering_[iEx][idirX], dirX ) ;
    physicalStartIndex_[iEy][idirX]  = cellNbrMin( hybridQtyCentering_[iEy][idirX], dirX ) ;
    physicalStartIndex_[iEz][idirX]  = cellNbrMin( hybridQtyCentering_[iEz][idirX], dirX ) ;

    physicalStartIndex_[iEx][idirY]  = cellNbrMin( hybridQtyCentering_[iEx][idirY], dirY ) ;
    physicalStartIndex_[iEy][idirY]  = cellNbrMin( hybridQtyCentering_[iEy][idirY], dirY ) ;
    physicalStartIndex_[iEz][idirY]  = cellNbrMin( hybridQtyCentering_[iEz][idirY], dirY ) ;

    physicalStartIndex_[iEx][idirZ]  = cellNbrMin( hybridQtyCentering_[iEx][idirZ], dirZ ) ;
    physicalStartIndex_[iEy][idirZ]  = cellNbrMin( hybridQtyCentering_[iEy][idirZ], dirZ ) ;
    physicalStartIndex_[iEz][idirZ]  = cellNbrMin( hybridQtyCentering_[iEz][idirZ], dirZ ) ;

    physicalStartIndex_[irho][idirX] = cellNbrMin( hybridQtyCentering_[irho][idirX], dirX ) ;
    physicalStartIndex_[irho][idirY] = cellNbrMin( hybridQtyCentering_[irho][idirY], dirY ) ;
    physicalStartIndex_[irho][idirZ] = cellNbrMin( hybridQtyCentering_[irho][idirZ], dirZ ) ;

    physicalStartIndex_[iV  ][idirX] = cellNbrMin( hybridQtyCentering_[iV  ][idirX], dirX ) ;
    physicalStartIndex_[iV  ][idirY] = cellNbrMin( hybridQtyCentering_[iV  ][idirY], dirY ) ;
    physicalStartIndex_[iV  ][idirZ] = cellNbrMin( hybridQtyCentering_[iV  ][idirZ], dirZ ) ;

    physicalStartIndex_[iP  ][idirX] = cellNbrMin( hybridQtyCentering_[iP  ][idirX], dirX ) ;
    physicalStartIndex_[iP  ][idirY] = cellNbrMin( hybridQtyCentering_[iP  ][idirY], dirY ) ;
    physicalStartIndex_[iP  ][idirZ] = cellNbrMin( hybridQtyCentering_[iP  ][idirZ], dirZ ) ;


    physicalEndIndex_[iBx][idirX]  = cellNbrMax( hybridQtyCentering_[iBx][idirX], dirX ) ;
    physicalEndIndex_[iBx][idirY]  = cellNbrMax( hybridQtyCentering_[iBx][idirY], dirY ) ;
    physicalEndIndex_[iBx][idirZ]  = cellNbrMax( hybridQtyCentering_[iBx][idirZ], dirZ ) ;

    physicalEndIndex_[iBy][idirX]  = cellNbrMax( hybridQtyCentering_[iBy][idirX], dirX ) ;
    physicalEndIndex_[iBy][idirY]  = cellNbrMax( hybridQtyCentering_[iBy][idirY], dirY ) ;
    physicalEndIndex_[iBy][idirZ]  = cellNbrMax( hybridQtyCentering_[iBy][idirZ], dirZ ) ;

    physicalEndIndex_[iBz][idirX]  = cellNbrMax( hybridQtyCentering_[iBz][idirX], dirX ) ;
    physicalEndIndex_[iBz][idirY]  = cellNbrMax( hybridQtyCentering_[iBz][idirY], dirY ) ;
    physicalEndIndex_[iBz][idirZ]  = cellNbrMax( hybridQtyCentering_[iBz][idirZ], dirZ ) ;

    physicalEndIndex_[iEx][idirX]  = cellNbrMax( hybridQtyCentering_[iEx][idirX], dirX ) ;
    physicalEndIndex_[iEx][idirY]  = cellNbrMax( hybridQtyCentering_[iEx][idirY], dirY ) ;
    physicalEndIndex_[iEx][idirZ]  = cellNbrMax( hybridQtyCentering_[iEx][idirZ], dirZ ) ;

    physicalEndIndex_[iEy][idirX]  = cellNbrMax( hybridQtyCentering_[iEy][idirX], dirX ) ;
    physicalEndIndex_[iEy][idirY]  = cellNbrMax( hybridQtyCentering_[iEy][idirY], dirY ) ;
    physicalEndIndex_[iEy][idirZ]  = cellNbrMax( hybridQtyCentering_[iEy][idirZ], dirZ ) ;

    physicalEndIndex_[iEz][idirX]  = cellNbrMax( hybridQtyCentering_[iEz][idirX], dirX ) ;
    physicalEndIndex_[iEz][idirY]  = cellNbrMax( hybridQtyCentering_[iEz][idirY], dirY ) ;
    physicalEndIndex_[iEz][idirZ]  = cellNbrMax( hybridQtyCentering_[iEz][idirZ], dirZ ) ;

    physicalEndIndex_[irho][idirX] = cellNbrMax( hybridQtyCentering_[irho][idirX], dirX ) ;
    physicalEndIndex_[irho][idirY] = cellNbrMax( hybridQtyCentering_[irho][idirY], dirY ) ;
    physicalEndIndex_[irho][idirZ] = cellNbrMax( hybridQtyCentering_[irho][idirZ], dirZ ) ;

    physicalEndIndex_[iV  ][idirX] = cellNbrMax( hybridQtyCentering_[iV  ][idirX], dirX ) ;
    physicalEndIndex_[iV  ][idirY] = cellNbrMax( hybridQtyCentering_[iV  ][idirY], dirY ) ;
    physicalEndIndex_[iV  ][idirZ] = cellNbrMax( hybridQtyCentering_[iV  ][idirZ], dirZ ) ;

    physicalEndIndex_[iP  ][idirX] = cellNbrMax( hybridQtyCentering_[iP  ][idirX], dirX ) ;
    physicalEndIndex_[iP  ][idirY] = cellNbrMax( hybridQtyCentering_[iP  ][idirY], dirY ) ;
    physicalEndIndex_[iP  ][idirZ] = cellNbrMax( hybridQtyCentering_[iP  ][idirZ], dirZ ) ;


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


    ghostEndIndex_[iBx][idirX]  = ghostCellNbrMax( hybridQtyCentering_[iBx][idirX], dirX ) ;
    ghostEndIndex_[iBx][idirY]  = ghostCellNbrMax( hybridQtyCentering_[iBx][idirY], dirY ) ;
    ghostEndIndex_[iBx][idirZ]  = ghostCellNbrMax( hybridQtyCentering_[iBx][idirZ], dirZ ) ;

    ghostEndIndex_[iBy][idirX]  = ghostCellNbrMax( hybridQtyCentering_[iBy][idirX], dirX ) ;
    ghostEndIndex_[iBy][idirY]  = ghostCellNbrMax( hybridQtyCentering_[iBy][idirY], dirY ) ;
    ghostEndIndex_[iBy][idirZ]  = ghostCellNbrMax( hybridQtyCentering_[iBy][idirZ], dirZ ) ;

    ghostEndIndex_[iBz][idirX]  = ghostCellNbrMax( hybridQtyCentering_[iBz][idirX], dirX ) ;
    ghostEndIndex_[iBz][idirY]  = ghostCellNbrMax( hybridQtyCentering_[iBz][idirY], dirY ) ;
    ghostEndIndex_[iBz][idirZ]  = ghostCellNbrMax( hybridQtyCentering_[iBz][idirZ], dirZ ) ;

    ghostEndIndex_[iEx][idirX]  = ghostCellNbrMax( hybridQtyCentering_[iEx][idirX], dirX ) ;
    ghostEndIndex_[iEx][idirY]  = ghostCellNbrMax( hybridQtyCentering_[iEx][idirY], dirY ) ;
    ghostEndIndex_[iEx][idirZ]  = ghostCellNbrMax( hybridQtyCentering_[iEx][idirZ], dirZ ) ;

    ghostEndIndex_[iEy][idirX]  = ghostCellNbrMax( hybridQtyCentering_[iEy][idirX], dirX ) ;
    ghostEndIndex_[iEy][idirY]  = ghostCellNbrMax( hybridQtyCentering_[iEy][idirY], dirY ) ;
    ghostEndIndex_[iEy][idirZ]  = ghostCellNbrMax( hybridQtyCentering_[iEy][idirZ], dirZ ) ;

    ghostEndIndex_[iEz][idirX]  = ghostCellNbrMax( hybridQtyCentering_[iEz][idirX], dirX ) ;
    ghostEndIndex_[iEz][idirY]  = ghostCellNbrMax( hybridQtyCentering_[iEz][idirY], dirY ) ;
    ghostEndIndex_[iEz][idirZ]  = ghostCellNbrMax( hybridQtyCentering_[iEz][idirZ], dirZ ) ;

    ghostEndIndex_[irho][idirX] = ghostCellNbrMax( hybridQtyCentering_[irho][idirX], dirX ) ;
    ghostEndIndex_[irho][idirY] = ghostCellNbrMax( hybridQtyCentering_[irho][idirY], dirY ) ;
    ghostEndIndex_[irho][idirZ] = ghostCellNbrMax( hybridQtyCentering_[irho][idirZ], dirZ ) ;

    ghostEndIndex_[iV  ][idirX] = ghostCellNbrMax( hybridQtyCentering_[iV  ][idirX], dirX ) ;
    ghostEndIndex_[iV  ][idirY] = ghostCellNbrMax( hybridQtyCentering_[iV  ][idirY], dirY ) ;
    ghostEndIndex_[iV  ][idirZ] = ghostCellNbrMax( hybridQtyCentering_[iV  ][idirZ], dirZ ) ;

    ghostEndIndex_[iP  ][idirX] = ghostCellNbrMax( hybridQtyCentering_[iP  ][idirX], dirX ) ;
    ghostEndIndex_[iP  ][idirY] = ghostCellNbrMax( hybridQtyCentering_[iP  ][idirY], dirY ) ;
    ghostEndIndex_[iP  ][idirZ] = ghostCellNbrMax( hybridQtyCentering_[iP  ][idirZ], dirZ ) ;

}


std::array<AllocSizeT ,3> GridLayoutImplYee::allocSize( EMFieldType fieldType ) const
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


    std::array<Alloc ,3> VecFieldSizes{
                   {Alloc(VFx_nx, VFx_ny, VFx_nz),
                    Alloc(VFy_nx, VFy_ny, VFy_nz),
                    Alloc(VFz_nx, VFz_ny, VFz_nz)} } ;

    return VecFieldSizes ;
}



AllocSizeT  GridLayoutImplYee::allocSize( LayoutType layout ) const
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


void GridLayoutImplYee::deriv(Field const& operand, Direction direction, Field& derivative) const
{
}



