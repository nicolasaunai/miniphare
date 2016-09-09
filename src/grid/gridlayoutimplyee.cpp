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
    hybridQtyCentering_[iBy][idirX]  = dual   ;
    hybridQtyCentering_[iBz][idirX]  = dual   ;

    hybridQtyCentering_[iBx][idirY]  = dual   ;
    hybridQtyCentering_[iBy][idirY]  = primal ;
    hybridQtyCentering_[iBz][idirY]  = dual   ;

    hybridQtyCentering_[iBx][idirZ]  = dual   ;
    hybridQtyCentering_[iBy][idirZ]  = dual   ;
    hybridQtyCentering_[iBz][idirZ]  = primal ;

    hybridQtyCentering_[iEx][idirX]  = dual   ;
    hybridQtyCentering_[iEy][idirX]  = primal ;
    hybridQtyCentering_[iEz][idirX]  = primal ;

    hybridQtyCentering_[iEx][idirY]  = primal ;
    hybridQtyCentering_[iEy][idirY]  = dual   ;
    hybridQtyCentering_[iEz][idirY]  = primal ;

    hybridQtyCentering_[iEx][idirZ]  = primal ;
    hybridQtyCentering_[iEy][idirZ]  = primal ;
    hybridQtyCentering_[iEz][idirZ]  = dual   ;

    hybridQtyCentering_[irho][idirX] = primal ;
    hybridQtyCentering_[iV][idirX]   = primal ;
    hybridQtyCentering_[iP][idirX]   = primal ;

    hybridQtyCentering_[irho][idirY] = primal ;
    hybridQtyCentering_[iV][idirY]   = primal ;
    hybridQtyCentering_[iP][idirY]   = primal ;

    hybridQtyCentering_[irho][idirZ] = primal ;
    hybridQtyCentering_[iV][idirZ]   = primal ;
    hybridQtyCentering_[iP][idirZ]   = primal ;


    physicalStartIndex_[iBx][idirX]  = primalCellNbrMin( dirX ) ;
    physicalStartIndex_[iBy][idirX]  = dualCellNbrMin  ( dirX ) ;
    physicalStartIndex_[iBz][idirX]  = dualCellNbrMin  ( dirX ) ;

    physicalStartIndex_[iBx][idirY]  = dualCellNbrMin  ( dirY ) ;
    physicalStartIndex_[iBy][idirY]  = primalCellNbrMin( dirY ) ;
    physicalStartIndex_[iBz][idirY]  = dualCellNbrMin  ( dirY ) ;

    physicalStartIndex_[iBx][idirZ]  = dualCellNbrMin  ( dirZ ) ;
    physicalStartIndex_[iBy][idirZ]  = dualCellNbrMin  ( dirZ ) ;
    physicalStartIndex_[iBz][idirZ]  = primalCellNbrMin( dirZ ) ;

    physicalStartIndex_[iEx][idirX]  = dualCellNbrMin  ( dirX ) ;
    physicalStartIndex_[iEy][idirX]  = primalCellNbrMin( dirX ) ;
    physicalStartIndex_[iEz][idirX]  = primalCellNbrMin( dirX ) ;

    physicalStartIndex_[iEx][idirY]  = primalCellNbrMin( dirY ) ;
    physicalStartIndex_[iEy][idirY]  = dualCellNbrMin  ( dirY ) ;
    physicalStartIndex_[iEz][idirY]  = primalCellNbrMin( dirY ) ;

    physicalStartIndex_[iEx][idirZ]  = primalCellNbrMin( dirZ ) ;
    physicalStartIndex_[iEy][idirZ]  = primalCellNbrMin( dirZ ) ;
    physicalStartIndex_[iEz][idirZ]  = dualCellNbrMin  ( dirZ ) ;

    physicalStartIndex_[irho][idirX] = primalCellNbrMin( dirX ) ;
    physicalStartIndex_[iV][idirX]   = primalCellNbrMin( dirX ) ;
    physicalStartIndex_[iP][idirX]   = primalCellNbrMin( dirX ) ;

    physicalStartIndex_[irho][idirY] = primalCellNbrMin( dirY ) ;
    physicalStartIndex_[iV][idirY]   = primalCellNbrMin( dirY ) ;
    physicalStartIndex_[iP][idirY]   = primalCellNbrMin( dirY ) ;

    physicalStartIndex_[irho][idirZ] = primalCellNbrMin( dirZ ) ;
    physicalStartIndex_[iV][idirZ]   = primalCellNbrMin( dirZ ) ;
    physicalStartIndex_[iP][idirZ]   = primalCellNbrMin( dirZ ) ;


    physicalEndIndex_[iBx][idirX]  = primalCellNbrMax( dirX ) ;
    physicalEndIndex_[iBy][idirX]  = dualCellNbrMax  ( dirX ) ;
    physicalEndIndex_[iBz][idirX]  = dualCellNbrMax  ( dirX ) ;

    physicalEndIndex_[iBx][idirY]  = dualCellNbrMax  ( dirY ) ;
    physicalEndIndex_[iBy][idirY]  = primalCellNbrMax( dirY ) ;
    physicalEndIndex_[iBz][idirY]  = dualCellNbrMax  ( dirY ) ;

    physicalEndIndex_[iBx][idirZ]  = dualCellNbrMax  ( dirZ ) ;
    physicalEndIndex_[iBy][idirZ]  = dualCellNbrMax  ( dirZ ) ;
    physicalEndIndex_[iBz][idirZ]  = primalCellNbrMax( dirZ ) ;

    physicalEndIndex_[iEx][idirX]  = dualCellNbrMax  ( dirX ) ;
    physicalEndIndex_[iEy][idirX]  = primalCellNbrMax( dirX ) ;
    physicalEndIndex_[iEz][idirX]  = primalCellNbrMax( dirX ) ;

    physicalEndIndex_[iEx][idirY]  = primalCellNbrMax( dirY ) ;
    physicalEndIndex_[iEy][idirY]  = dualCellNbrMax  ( dirY ) ;
    physicalEndIndex_[iEz][idirY]  = primalCellNbrMax( dirY ) ;

    physicalEndIndex_[iEx][idirZ]  = primalCellNbrMax( dirZ ) ;
    physicalEndIndex_[iEy][idirZ]  = primalCellNbrMax( dirZ ) ;
    physicalEndIndex_[iEz][idirZ]  = dualCellNbrMax  ( dirZ ) ;

    physicalEndIndex_[irho][idirX] = primalCellNbrMax( dirX ) ;
    physicalEndIndex_[iV][idirX]   = primalCellNbrMax( dirX ) ;
    physicalEndIndex_[iP][idirX]   = primalCellNbrMax( dirX ) ;

    physicalEndIndex_[irho][idirY] = primalCellNbrMax( dirY ) ;
    physicalEndIndex_[iV][idirY]   = primalCellNbrMax( dirY ) ;
    physicalEndIndex_[iP][idirY]   = primalCellNbrMax( dirY ) ;

    physicalEndIndex_[irho][idirZ] = primalCellNbrMax( dirZ ) ;
    physicalEndIndex_[iV][idirZ]   = primalCellNbrMax( dirZ ) ;
    physicalEndIndex_[iP][idirZ]   = primalCellNbrMax( dirZ ) ;


    ghostStartIndex_[iBx][idirX]  = nbrPaddingCells( dirX ) ;
    ghostStartIndex_[iBy][idirX]  = nbrPaddingCells( dirX ) ;
    ghostStartIndex_[iBz][idirX]  = nbrPaddingCells( dirX ) ;

    ghostStartIndex_[iBx][idirY]  = nbrPaddingCells( dirY ) ;
    ghostStartIndex_[iBy][idirY]  = nbrPaddingCells( dirY ) ;
    ghostStartIndex_[iBz][idirY]  = nbrPaddingCells( dirY ) ;

    ghostStartIndex_[iBx][idirZ]  = nbrPaddingCells( dirZ ) ;
    ghostStartIndex_[iBy][idirZ]  = nbrPaddingCells( dirZ ) ;
    ghostStartIndex_[iBz][idirZ]  = nbrPaddingCells( dirZ ) ;

    ghostStartIndex_[iEx][idirX]  = nbrPaddingCells( dirX ) ;
    ghostStartIndex_[iEy][idirX]  = nbrPaddingCells( dirX ) ;
    ghostStartIndex_[iEz][idirX]  = nbrPaddingCells( dirX ) ;

    ghostStartIndex_[iEx][idirY]  = nbrPaddingCells( dirY ) ;
    ghostStartIndex_[iEy][idirY]  = nbrPaddingCells( dirY ) ;
    ghostStartIndex_[iEz][idirY]  = nbrPaddingCells( dirY ) ;

    ghostStartIndex_[iEx][idirZ]  = nbrPaddingCells( dirZ ) ;
    ghostStartIndex_[iEy][idirZ]  = nbrPaddingCells( dirZ ) ;
    ghostStartIndex_[iEz][idirZ]  = nbrPaddingCells( dirZ ) ;

    ghostStartIndex_[irho][idirX] = nbrPaddingCells( dirX ) ;
    ghostStartIndex_[iV][idirX]   = nbrPaddingCells( dirX ) ;
    ghostStartIndex_[iP][idirX]   = nbrPaddingCells( dirX ) ;

    ghostStartIndex_[irho][idirY] = nbrPaddingCells( dirY ) ;
    ghostStartIndex_[iV][idirY]   = nbrPaddingCells( dirY ) ;
    ghostStartIndex_[iP][idirY]   = nbrPaddingCells( dirY ) ;

    ghostStartIndex_[irho][idirZ] = nbrPaddingCells( dirZ ) ;
    ghostStartIndex_[iV][idirZ]   = nbrPaddingCells( dirZ ) ;
    ghostStartIndex_[iP][idirZ]   = nbrPaddingCells( dirZ ) ;


    ghostEndIndex_[iBx][idirX]  = primalGhostCellNbrMax( dirX ) ;
    ghostEndIndex_[iBy][idirX]  = dualGhostCellNbrMax  ( dirX ) ;
    ghostEndIndex_[iBz][idirX]  = dualGhostCellNbrMax  ( dirX ) ;

    ghostEndIndex_[iBx][idirY]  = dualGhostCellNbrMax  ( dirY ) ;
    ghostEndIndex_[iBy][idirY]  = primalGhostCellNbrMax( dirY ) ;
    ghostEndIndex_[iBz][idirY]  = dualGhostCellNbrMax  ( dirY ) ;

    ghostEndIndex_[iBx][idirZ]  = dualGhostCellNbrMax  ( dirZ ) ;
    ghostEndIndex_[iBy][idirZ]  = dualGhostCellNbrMax  ( dirZ ) ;
    ghostEndIndex_[iBz][idirZ]  = primalGhostCellNbrMax( dirZ ) ;

    ghostEndIndex_[iEx][idirX]  = dualGhostCellNbrMax  ( dirX ) ;
    ghostEndIndex_[iEy][idirX]  = primalGhostCellNbrMax( dirX ) ;
    ghostEndIndex_[iEz][idirX]  = primalGhostCellNbrMax( dirX ) ;

    ghostEndIndex_[iEx][idirY]  = primalGhostCellNbrMax( dirY ) ;
    ghostEndIndex_[iEy][idirY]  = dualGhostCellNbrMax  ( dirY ) ;
    ghostEndIndex_[iEz][idirY]  = primalGhostCellNbrMax( dirY ) ;

    ghostEndIndex_[iEx][idirZ]  = primalGhostCellNbrMax( dirZ ) ;
    ghostEndIndex_[iEy][idirZ]  = primalGhostCellNbrMax( dirZ ) ;
    ghostEndIndex_[iEz][idirZ]  = dualGhostCellNbrMax  ( dirZ ) ;

    ghostEndIndex_[irho][idirX] = primalGhostCellNbrMax( dirX ) ;
    ghostEndIndex_[iV][idirX]   = primalGhostCellNbrMax( dirX ) ;
    ghostEndIndex_[iP][idirX]   = primalGhostCellNbrMax( dirX ) ;

    ghostEndIndex_[irho][idirY] = primalGhostCellNbrMax( dirY ) ;
    ghostEndIndex_[iV][idirY]   = primalGhostCellNbrMax( dirY ) ;
    ghostEndIndex_[iP][idirY]   = primalGhostCellNbrMax( dirY ) ;

    ghostEndIndex_[irho][idirZ] = primalGhostCellNbrMax( dirZ ) ;
    ghostEndIndex_[iV][idirZ]   = primalGhostCellNbrMax( dirZ ) ;
    ghostEndIndex_[iP][idirZ]   = primalGhostCellNbrMax( dirZ ) ;

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


std::array<AllocSizeT ,3> GridLayoutImplYee::allocSize( OhmTerm term ) const
{


}

AllocSizeT GridLayoutImplYee::allocSize( DerivedEMField derivedField ) const
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



