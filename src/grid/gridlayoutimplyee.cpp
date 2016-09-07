#include <stdexcept>

#include "gridlayoutimplyee.h"




GridLayoutImplYee::GridLayoutImplYee(uint32 nbDims,
                                     uint32 interpOrder,
                                     std::array<uint32,3> nbrCellsXYZ)
    : GridLayoutImplInternals(nbDims, interpOrder, nbrCellsXYZ)
{
    Direction dirX = Direction::directionX ;
    Direction dirY = Direction::directionY ;
    Direction dirZ = Direction::directionZ ;

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

    uint32 iBx = static_cast<uint32>(HybridQuantity::Bx) ;
    uint32 iBy = static_cast<uint32>(HybridQuantity::By) ;
    uint32 iBz = static_cast<uint32>(HybridQuantity::Bz) ;

    uint32 iEx = static_cast<uint32>(HybridQuantity::Ex) ;
    uint32 iEy = static_cast<uint32>(HybridQuantity::Ey) ;
    uint32 iEz = static_cast<uint32>(HybridQuantity::Ez) ;

    uint32 VFx_nx, VFx_ny, VFx_nz ;
    uint32 VFy_nx, VFy_ny, VFy_nz ;
    uint32 VFz_nx, VFz_ny, VFz_nz ;

    switch( fieldType )
    {
    case EMFieldType::EVecField:
        VFx_nx = ghostEndIndex_[iEx][idirX] - ghostStartIndex_[iEx][idirX] + 1
                + 2*nbrPaddingCells(dirX) ;
        VFx_ny = ghostEndIndex_[iEx][idirY] - ghostStartIndex_[iEx][idirY] + 1
                + 2*nbrPaddingCells(dirY) ;
        VFx_nz = ghostEndIndex_[iEx][idirZ] - ghostStartIndex_[iEx][idirZ] + 1
                + 2*nbrPaddingCells(dirZ) ;
        VFy_nx = ghostEndIndex_[iEy][idirX] - ghostStartIndex_[iEy][idirX] + 1
                + 2*nbrPaddingCells(dirX) ;
        VFy_ny = ghostEndIndex_[iEy][idirY] - ghostStartIndex_[iEy][idirY] + 1
                + 2*nbrPaddingCells(dirY) ;
        VFy_nz = ghostEndIndex_[iEy][idirZ] - ghostStartIndex_[iEy][idirZ] + 1
                + 2*nbrPaddingCells(dirZ) ;
        VFz_nx = ghostEndIndex_[iEz][idirX] - ghostStartIndex_[iEz][idirX] + 1
                + 2*nbrPaddingCells(dirX) ;
        VFz_ny = ghostEndIndex_[iEz][idirY] - ghostStartIndex_[iEz][idirY] + 1
                + 2*nbrPaddingCells(dirY) ;
        VFz_nz = ghostEndIndex_[iEz][idirZ] - ghostStartIndex_[iEz][idirZ] + 1
                + 2*nbrPaddingCells(dirZ) ;

        break;

    case EMFieldType::BVecField:
        VFx_nx = ghostEndIndex_[iBx][idirX] - ghostStartIndex_[iBx][idirX] + 1
                + 2*nbrPaddingCells(dirX) ;
        VFx_ny = ghostEndIndex_[iBx][idirY] - ghostStartIndex_[iBx][idirY] + 1
                + 2*nbrPaddingCells(dirY) ;
        VFx_nz = ghostEndIndex_[iBx][idirZ] - ghostStartIndex_[iBx][idirZ] + 1
                + 2*nbrPaddingCells(dirZ) ;
        VFy_nx = ghostEndIndex_[iBy][idirX] - ghostStartIndex_[iBy][idirX] + 1
                + 2*nbrPaddingCells(dirX) ;
        VFy_ny = ghostEndIndex_[iBy][idirY] - ghostStartIndex_[iBy][idirY] + 1
                + 2*nbrPaddingCells(dirY) ;
        VFy_nz = ghostEndIndex_[iBy][idirZ] - ghostStartIndex_[iBy][idirZ] + 1
                + 2*nbrPaddingCells(dirZ) ;
        VFz_nx = ghostEndIndex_[iBz][idirX] - ghostStartIndex_[iBz][idirX] + 1
                + 2*nbrPaddingCells(dirX) ;
        VFz_ny = ghostEndIndex_[iBz][idirY] - ghostStartIndex_[iBz][idirY] + 1
                + 2*nbrPaddingCells(dirY) ;
        VFz_nz = ghostEndIndex_[iBz][idirZ] - ghostStartIndex_[iBz][idirZ] + 1
                + 2*nbrPaddingCells(dirZ) ;

        break;
    }

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



