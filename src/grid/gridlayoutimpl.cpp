
#include "gridlayoutimpl.h"

#include "hybridenums.h"

#include <cmath>
#include <stdexcept>
#include <array>


GridLayoutImplInternals::GridLayoutImplInternals(uint32 nbDims, uint32 ghostParameter,
                                                 std::array<uint32,3> nbrCellsXYZ ,
                                                 std::array<double,3> dxdydz      )
    : nbdims_{nbDims},
      nbrCellx_{nbrCellsXYZ[0]}, nbrCelly_{nbrCellsXYZ[1]}, nbrCellz_{nbrCellsXYZ[2]},
      nbrPaddingCellsX_{defaultNbrPaddingCells},
      nbrPaddingCellsY_{defaultNbrPaddingCells},
      nbrPaddingCellsZ_{defaultNbrPaddingCells},
      dx_{dxdydz[0]}, dy_{dxdydz[1]}, dz_{dxdydz[2]},
      odxdydz_{ {1./dx_, 1./dy_, 1./dz_} }
{
    computeNbrGhosts( ghostParameter );
}





void GridLayoutImplInternals::initGridUtils( const gridDataT & data )
{
    nbrPaddingCells_[data.idirX] = nbrPaddingCellsX_ ;
    nbrPaddingCells_[data.idirY] = nbrPaddingCellsY_ ;
    nbrPaddingCells_[data.idirZ] = nbrPaddingCellsZ_ ;

    nbrPhysicalCells_[data.idirX] = nbrCellx_ ;
    nbrPhysicalCells_[data.idirY] = nbrCelly_ ;
    nbrPhysicalCells_[data.idirZ] = nbrCellz_ ;

}




void GridLayoutImplInternals::initPhysicalStart( const gridDataT & data )
{
    physicalStartIndex_[data.iBx][data.idirX]  = cellIndexAtMin( hybridQtyCentering_[data.iBx][data.idirX], data.dirX ) ;
    physicalStartIndex_[data.iBx][data.idirY]  = cellIndexAtMin( hybridQtyCentering_[data.iBx][data.idirY], data.dirY ) ;
    physicalStartIndex_[data.iBx][data.idirZ]  = cellIndexAtMin( hybridQtyCentering_[data.iBx][data.idirZ], data.dirZ ) ;

    physicalStartIndex_[data.iBy][data.idirX]  = cellIndexAtMin( hybridQtyCentering_[data.iBy][data.idirX], data.dirX ) ;
    physicalStartIndex_[data.iBy][data.idirY]  = cellIndexAtMin( hybridQtyCentering_[data.iBy][data.idirY], data.dirY ) ;
    physicalStartIndex_[data.iBy][data.idirZ]  = cellIndexAtMin( hybridQtyCentering_[data.iBy][data.idirZ], data.dirZ ) ;

    physicalStartIndex_[data.iBz][data.idirX]  = cellIndexAtMin( hybridQtyCentering_[data.iBz][data.idirX], data.dirX ) ;
    physicalStartIndex_[data.iBz][data.idirY]  = cellIndexAtMin( hybridQtyCentering_[data.iBz][data.idirY], data.dirY ) ;
    physicalStartIndex_[data.iBz][data.idirZ]  = cellIndexAtMin( hybridQtyCentering_[data.iBz][data.idirZ], data.dirZ ) ;

    physicalStartIndex_[data.iEx][data.idirX]  = cellIndexAtMin( hybridQtyCentering_[data.iEx][data.idirX], data.dirX ) ;
    physicalStartIndex_[data.iEx][data.idirY]  = cellIndexAtMin( hybridQtyCentering_[data.iEx][data.idirY], data.dirY ) ;
    physicalStartIndex_[data.iEx][data.idirZ]  = cellIndexAtMin( hybridQtyCentering_[data.iEx][data.idirZ], data.dirZ ) ;

    physicalStartIndex_[data.iEy][data.idirX]  = cellIndexAtMin( hybridQtyCentering_[data.iEy][data.idirX], data.dirX ) ;
    physicalStartIndex_[data.iEy][data.idirY]  = cellIndexAtMin( hybridQtyCentering_[data.iEy][data.idirY], data.dirY ) ;
    physicalStartIndex_[data.iEy][data.idirZ]  = cellIndexAtMin( hybridQtyCentering_[data.iEy][data.idirZ], data.dirZ ) ;

    physicalStartIndex_[data.iEz][data.idirX]  = cellIndexAtMin( hybridQtyCentering_[data.iEz][data.idirX], data.dirX ) ;
    physicalStartIndex_[data.iEz][data.idirY]  = cellIndexAtMin( hybridQtyCentering_[data.iEz][data.idirY], data.dirY ) ;
    physicalStartIndex_[data.iEz][data.idirZ]  = cellIndexAtMin( hybridQtyCentering_[data.iEz][data.idirZ], data.dirZ ) ;

    physicalStartIndex_[data.irho][data.idirX] = cellIndexAtMin( hybridQtyCentering_[data.irho][data.idirX], data.dirX ) ;
    physicalStartIndex_[data.irho][data.idirY] = cellIndexAtMin( hybridQtyCentering_[data.irho][data.idirY], data.dirY ) ;
    physicalStartIndex_[data.irho][data.idirZ] = cellIndexAtMin( hybridQtyCentering_[data.irho][data.idirZ], data.dirZ ) ;

    physicalStartIndex_[data.iV  ][data.idirX] = cellIndexAtMin( hybridQtyCentering_[data.iV  ][data.idirX], data.dirX ) ;
    physicalStartIndex_[data.iV  ][data.idirY] = cellIndexAtMin( hybridQtyCentering_[data.iV  ][data.idirY], data.dirY ) ;
    physicalStartIndex_[data.iV  ][data.idirZ] = cellIndexAtMin( hybridQtyCentering_[data.iV  ][data.idirZ], data.dirZ ) ;

    physicalStartIndex_[data.iP  ][data.idirX] = cellIndexAtMin( hybridQtyCentering_[data.iP  ][data.idirX], data.dirX ) ;
    physicalStartIndex_[data.iP  ][data.idirY] = cellIndexAtMin( hybridQtyCentering_[data.iP  ][data.idirY], data.dirY ) ;
    physicalStartIndex_[data.iP  ][data.idirZ] = cellIndexAtMin( hybridQtyCentering_[data.iP  ][data.idirZ], data.dirZ ) ;

}

void GridLayoutImplInternals::initPhysicalEnd( const gridDataT & data )
{

    physicalEndIndex_[data.iBx][data.idirX]  = cellIndexAtMax( hybridQtyCentering_[data.iBx][data.idirX], data.dirX ) ;
    physicalEndIndex_[data.iBx][data.idirY]  = cellIndexAtMax( hybridQtyCentering_[data.iBx][data.idirY], data.dirY ) ;
    physicalEndIndex_[data.iBx][data.idirZ]  = cellIndexAtMax( hybridQtyCentering_[data.iBx][data.idirZ], data.dirZ ) ;

    physicalEndIndex_[data.iBy][data.idirX]  = cellIndexAtMax( hybridQtyCentering_[data.iBy][data.idirX], data.dirX ) ;
    physicalEndIndex_[data.iBy][data.idirY]  = cellIndexAtMax( hybridQtyCentering_[data.iBy][data.idirY], data.dirY ) ;
    physicalEndIndex_[data.iBy][data.idirZ]  = cellIndexAtMax( hybridQtyCentering_[data.iBy][data.idirZ], data.dirZ ) ;

    physicalEndIndex_[data.iBz][data.idirX]  = cellIndexAtMax( hybridQtyCentering_[data.iBz][data.idirX], data.dirX ) ;
    physicalEndIndex_[data.iBz][data.idirY]  = cellIndexAtMax( hybridQtyCentering_[data.iBz][data.idirY], data.dirY ) ;
    physicalEndIndex_[data.iBz][data.idirZ]  = cellIndexAtMax( hybridQtyCentering_[data.iBz][data.idirZ], data.dirZ ) ;

    physicalEndIndex_[data.iEx][data.idirX]  = cellIndexAtMax( hybridQtyCentering_[data.iEx][data.idirX], data.dirX ) ;
    physicalEndIndex_[data.iEx][data.idirY]  = cellIndexAtMax( hybridQtyCentering_[data.iEx][data.idirY], data.dirY ) ;
    physicalEndIndex_[data.iEx][data.idirZ]  = cellIndexAtMax( hybridQtyCentering_[data.iEx][data.idirZ], data.dirZ ) ;

    physicalEndIndex_[data.iEy][data.idirX]  = cellIndexAtMax( hybridQtyCentering_[data.iEy][data.idirX], data.dirX ) ;
    physicalEndIndex_[data.iEy][data.idirY]  = cellIndexAtMax( hybridQtyCentering_[data.iEy][data.idirY], data.dirY ) ;
    physicalEndIndex_[data.iEy][data.idirZ]  = cellIndexAtMax( hybridQtyCentering_[data.iEy][data.idirZ], data.dirZ ) ;

    physicalEndIndex_[data.iEz][data.idirX]  = cellIndexAtMax( hybridQtyCentering_[data.iEz][data.idirX], data.dirX ) ;
    physicalEndIndex_[data.iEz][data.idirY]  = cellIndexAtMax( hybridQtyCentering_[data.iEz][data.idirY], data.dirY ) ;
    physicalEndIndex_[data.iEz][data.idirZ]  = cellIndexAtMax( hybridQtyCentering_[data.iEz][data.idirZ], data.dirZ ) ;

    physicalEndIndex_[data.irho][data.idirX] = cellIndexAtMax( hybridQtyCentering_[data.irho][data.idirX], data.dirX ) ;
    physicalEndIndex_[data.irho][data.idirY] = cellIndexAtMax( hybridQtyCentering_[data.irho][data.idirY], data.dirY ) ;
    physicalEndIndex_[data.irho][data.idirZ] = cellIndexAtMax( hybridQtyCentering_[data.irho][data.idirZ], data.dirZ ) ;

    physicalEndIndex_[data.iV  ][data.idirX] = cellIndexAtMax( hybridQtyCentering_[data.iV  ][data.idirX], data.dirX ) ;
    physicalEndIndex_[data.iV  ][data.idirY] = cellIndexAtMax( hybridQtyCentering_[data.iV  ][data.idirY], data.dirY ) ;
    physicalEndIndex_[data.iV  ][data.idirZ] = cellIndexAtMax( hybridQtyCentering_[data.iV  ][data.idirZ], data.dirZ ) ;

    physicalEndIndex_[data.iP  ][data.idirX] = cellIndexAtMax( hybridQtyCentering_[data.iP  ][data.idirX], data.dirX ) ;
    physicalEndIndex_[data.iP  ][data.idirY] = cellIndexAtMax( hybridQtyCentering_[data.iP  ][data.idirY], data.dirY ) ;
    physicalEndIndex_[data.iP  ][data.idirZ] = cellIndexAtMax( hybridQtyCentering_[data.iP  ][data.idirZ], data.dirZ ) ;

}

void GridLayoutImplInternals::initGhostStart( const gridDataT & data )
{

    ghostStartIndex_[data.iBx][data.idirX]  = nbrPaddingCells_[data.idirX] ;
    ghostStartIndex_[data.iBx][data.idirY]  = nbrPaddingCells_[data.idirY] ;
    ghostStartIndex_[data.iBx][data.idirZ]  = nbrPaddingCells_[data.idirZ] ;

    ghostStartIndex_[data.iBy][data.idirX]  = nbrPaddingCells_[data.idirX] ;
    ghostStartIndex_[data.iBy][data.idirY]  = nbrPaddingCells_[data.idirY] ;
    ghostStartIndex_[data.iBy][data.idirZ]  = nbrPaddingCells_[data.idirZ] ;

    ghostStartIndex_[data.iBz][data.idirX]  = nbrPaddingCells_[data.idirX] ;
    ghostStartIndex_[data.iBz][data.idirY]  = nbrPaddingCells_[data.idirY] ;
    ghostStartIndex_[data.iBz][data.idirZ]  = nbrPaddingCells_[data.idirZ] ;

    ghostStartIndex_[data.iEx][data.idirX]  = nbrPaddingCells_[data.idirX] ;
    ghostStartIndex_[data.iEx][data.idirY]  = nbrPaddingCells_[data.idirY] ;
    ghostStartIndex_[data.iEx][data.idirZ]  = nbrPaddingCells_[data.idirZ] ;

    ghostStartIndex_[data.iEy][data.idirX]  = nbrPaddingCells_[data.idirX] ;
    ghostStartIndex_[data.iEy][data.idirY]  = nbrPaddingCells_[data.idirY] ;
    ghostStartIndex_[data.iEy][data.idirZ]  = nbrPaddingCells_[data.idirZ] ;

    ghostStartIndex_[data.iEz][data.idirX]  = nbrPaddingCells_[data.idirX] ;
    ghostStartIndex_[data.iEz][data.idirY]  = nbrPaddingCells_[data.idirY] ;
    ghostStartIndex_[data.iEz][data.idirZ]  = nbrPaddingCells_[data.idirZ] ;

    ghostStartIndex_[data.irho][data.idirX] = nbrPaddingCells_[data.idirX] ;
    ghostStartIndex_[data.irho][data.idirY] = nbrPaddingCells_[data.idirY] ;
    ghostStartIndex_[data.irho][data.idirZ] = nbrPaddingCells_[data.idirZ] ;

    ghostStartIndex_[data.iV  ][data.idirX] = nbrPaddingCells_[data.idirX] ;
    ghostStartIndex_[data.iV  ][data.idirY] = nbrPaddingCells_[data.idirY] ;
    ghostStartIndex_[data.iV  ][data.idirZ] = nbrPaddingCells_[data.idirZ] ;

    ghostStartIndex_[data.iP  ][data.idirX] = nbrPaddingCells_[data.idirX] ;
    ghostStartIndex_[data.iP  ][data.idirY] = nbrPaddingCells_[data.idirY] ;
    ghostStartIndex_[data.iP  ][data.idirZ] = nbrPaddingCells_[data.idirZ] ;

}


void GridLayoutImplInternals::initGhostEnd( const gridDataT & data )
{

    ghostEndIndex_[data.iBx][data.idirX]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iBx][data.idirX], data.dirX ) ;
    ghostEndIndex_[data.iBx][data.idirY]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iBx][data.idirY], data.dirY ) ;
    ghostEndIndex_[data.iBx][data.idirZ]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iBx][data.idirZ], data.dirZ ) ;

    ghostEndIndex_[data.iBy][data.idirX]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iBy][data.idirX], data.dirX ) ;
    ghostEndIndex_[data.iBy][data.idirY]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iBy][data.idirY], data.dirY ) ;
    ghostEndIndex_[data.iBy][data.idirZ]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iBy][data.idirZ], data.dirZ ) ;

    ghostEndIndex_[data.iBz][data.idirX]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iBz][data.idirX], data.dirX ) ;
    ghostEndIndex_[data.iBz][data.idirY]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iBz][data.idirY], data.dirY ) ;
    ghostEndIndex_[data.iBz][data.idirZ]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iBz][data.idirZ], data.dirZ ) ;

    ghostEndIndex_[data.iEx][data.idirX]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iEx][data.idirX], data.dirX ) ;
    ghostEndIndex_[data.iEx][data.idirY]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iEx][data.idirY], data.dirY ) ;
    ghostEndIndex_[data.iEx][data.idirZ]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iEx][data.idirZ], data.dirZ ) ;

    ghostEndIndex_[data.iEy][data.idirX]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iEy][data.idirX], data.dirX ) ;
    ghostEndIndex_[data.iEy][data.idirY]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iEy][data.idirY], data.dirY ) ;
    ghostEndIndex_[data.iEy][data.idirZ]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iEy][data.idirZ], data.dirZ ) ;

    ghostEndIndex_[data.iEz][data.idirX]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iEz][data.idirX], data.dirX ) ;
    ghostEndIndex_[data.iEz][data.idirY]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iEz][data.idirY], data.dirY ) ;
    ghostEndIndex_[data.iEz][data.idirZ]  = ghostCellIndexAtMax( hybridQtyCentering_[data.iEz][data.idirZ], data.dirZ ) ;

    ghostEndIndex_[data.irho][data.idirX] = ghostCellIndexAtMax( hybridQtyCentering_[data.irho][data.idirX], data.dirX ) ;
    ghostEndIndex_[data.irho][data.idirY] = ghostCellIndexAtMax( hybridQtyCentering_[data.irho][data.idirY], data.dirY ) ;
    ghostEndIndex_[data.irho][data.idirZ] = ghostCellIndexAtMax( hybridQtyCentering_[data.irho][data.idirZ], data.dirZ ) ;

    ghostEndIndex_[data.iV  ][data.idirX] = ghostCellIndexAtMax( hybridQtyCentering_[data.iV  ][data.idirX], data.dirX ) ;
    ghostEndIndex_[data.iV  ][data.idirY] = ghostCellIndexAtMax( hybridQtyCentering_[data.iV  ][data.idirY], data.dirY ) ;
    ghostEndIndex_[data.iV  ][data.idirZ] = ghostCellIndexAtMax( hybridQtyCentering_[data.iV  ][data.idirZ], data.dirZ ) ;

    ghostEndIndex_[data.iP  ][data.idirX] = ghostCellIndexAtMax( hybridQtyCentering_[data.iP  ][data.idirX], data.dirX ) ;
    ghostEndIndex_[data.iP  ][data.idirY] = ghostCellIndexAtMax( hybridQtyCentering_[data.iP  ][data.idirY], data.dirY ) ;
    ghostEndIndex_[data.iP  ][data.idirZ] = ghostCellIndexAtMax( hybridQtyCentering_[data.iP  ][data.idirZ], data.dirZ ) ;

}


QtyCentering GridLayoutImplInternals::derivedCentering(HybridQuantity qty, Direction dir) const
{

    uint32 iField = static_cast<uint32>( qty ) ;
    uint32 idir   = static_cast<uint32>( dir ) ;

    QtyCentering newLayout = changeCentering( hybridQtyCentering_[iField][idir] ) ;

    return newLayout ;
}


AllocSizeT GridLayoutImplInternals::allocSize_( HybridQuantity qty ) const
{
    gridDataT data{} ;

    uint32 iQty = static_cast<uint32>(qty) ;

    std::array<QtyCentering, NBR_COMPO>
            qtyCenterings{ {hybridQtyCentering_[iQty][data.idirX],
                            hybridQtyCentering_[iQty][data.idirY],
                            hybridQtyCentering_[iQty][data.idirZ]} } ;

    auto allocSizes = computeSizes( qtyCenterings ) ;

    return AllocSizeT( allocSizes[0], allocSizes[1], allocSizes[2] );
}



AllocSizeT  GridLayoutImplInternals::allocSizeDerived_( HybridQuantity qty, Direction dir ) const
{
    gridDataT data{} ;

    uint32 iDerivedDir = static_cast<uint32>( dir ) ;

    uint32 iQty = static_cast<uint32>(qty) ;

    std::array<QtyCentering, NBR_COMPO>
            qtyCenterings{ {hybridQtyCentering_[iQty][data.idirX],
                            hybridQtyCentering_[iQty][data.idirY],
                            hybridQtyCentering_[iQty][data.idirZ]} } ;

    QtyCentering newCentering = derivedCentering( qty, dir ) ;

    qtyCenterings[iDerivedDir] = newCentering ;

    auto allocSizes = computeSizes( qtyCenterings ) ;

    return AllocSizeT( allocSizes[0], allocSizes[1], allocSizes[2] );
}



std::array<uint32, NBR_COMPO> GridLayoutImplInternals::computeSizes(
        std::array<QtyCentering, NBR_COMPO> const & qtyCenterings ) const
{
    gridDataT data{} ;

    uint32 nx =  2*nbrPaddingCells( Direction::X ) + nbrPhysicalCells( Direction::X ) + 1
               + 2*nbrGhosts( qtyCenterings[data.idirX] ) ;

    uint32 ny =  2*nbrPaddingCells( Direction::Y ) + nbrPhysicalCells( Direction::Y ) + 1
               + 2*nbrGhosts( qtyCenterings[data.idirY] ) ;

    uint32 nz =  2*nbrPaddingCells( Direction::Z ) + nbrPhysicalCells( Direction::Z ) + 1
               + 2*nbrGhosts( qtyCenterings[data.idirZ] ) ;

    return { {nx, ny, nz} } ;
}


// start and end index used in computing loops
uint32 GridLayoutImplInternals::physicalStartIndexV(Field const& field, Direction direction) const
{
    uint32 iQty = static_cast<uint32>(field.hybridQty());
    uint32 iDir = static_cast<uint32>(direction);
    return physicalStartIndex_[iQty][iDir];
}


uint32 GridLayoutImplInternals::physicalEndIndexV(Field const& field, Direction direction) const
{
    uint32 iQty = static_cast<uint32>(field.hybridQty());
    uint32 iDir = static_cast<uint32>(direction);
    return physicalEndIndex_[iQty][iDir];
}



uint32 GridLayoutImplInternals::ghostStartIndexV(Field const& field, Direction direction) const
{
    uint32 iQty = static_cast<uint32>(field.hybridQty());
    uint32 iDir = static_cast<uint32>(direction);
    return ghostStartIndex_[iQty][iDir];
}


uint32 GridLayoutImplInternals::ghostEndIndexV(Field const& field, Direction direction) const
{
    uint32 iQty = static_cast<uint32>(field.hybridQty());
    uint32 iDir = static_cast<uint32>(direction);
    return ghostEndIndex_[iQty][iDir];
}



QtyCentering GridLayoutImplInternals::changeCentering(QtyCentering centering ) const
{
    QtyCentering newCentering = QtyCentering::primal ;

    if( centering == QtyCentering::primal )
    {
        newCentering = QtyCentering::dual ;
    }

    return newCentering ;
}


/**
 * @brief fieldNodeCoordinates returns a Point,
 * the idea is to make in every initializer
 * method, 3 nested loops over primal PhysicalStart/End indices
 * @param field
 * the returned point depends on the field's centering
 * @param origin
 * @param ix is a primal index
 * @param iy is a primal index
 * @param iz is a primal index
 * @return Point
 * the desired field-centered coordinate
 */
Point GridLayoutImplInternals::fieldNodeCoordinates_(
        const Field & field, const Point & origin,
        uint32 ix, uint32 iy, uint32 iz ) const
{
    uint32 idirX = static_cast<uint32>(Direction::X) ;
    uint32 idirY = static_cast<uint32>(Direction::Y) ;
    uint32 idirZ = static_cast<uint32>(Direction::Z) ;

    uint32 ixStart = cellIndexAtMin( QtyCentering::primal, Direction::X ) ;
    uint32 iyStart = cellIndexAtMin( QtyCentering::primal, Direction::Y ) ;
    uint32 izStart = cellIndexAtMin( QtyCentering::primal, Direction::Z ) ;

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

    double x = ( (ix-ixStart) + halfCell[0])*dx_ + origin.x_ ;
    double y = ( (iy-iyStart) + halfCell[1])*dy_ + origin.y_ ;
    double z = ( (iz-izStart) + halfCell[2])*dz_ + origin.z_ ;

    return Point(x, y, z) ;
}


/**
 * @brief cellCenteredCoordinates returns a cell-centered Point.
 * The idea is to call this method in every initializer method
 * using 3 nested loops over primal PhysicalStart/End indices.
 * @param origin
 * @param ix is a primal index
 * @param iy is a primal index
 * @param iz is a primal index
 * @return Point
 * the desired cell-centered (dual/dual/dual) coordinate
 */
Point GridLayoutImplInternals::cellCenteredCoordinates_(
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

    double x = ( (ix-ixStart) + halfCell)*dx_ + origin.x_ ;
    double y = ( (iy-iyStart) + halfCell)*dy_ + origin.y_ ;
    double z = ( (iz-izStart) + halfCell)*dz_ + origin.z_ ;

    return Point(x, y, z) ;
}



/**
 * @brief computeOffsets computes the number of ghost cells for fields.
 * On the primal mesh the number of ghosts depends on centeredOffset_
 * On the dual mesh the number of ghosts depends on
 * leftOffset_ and rightOffset_
 *
 * This is explained in details on the wiki page
 * https://hephaistos.lpp.polytechnique.fr/redmine/projects/hyb-par/wiki/PrimalDual
 *
 * @param ghostParameter, corresponds to the interpolation order
 */
void GridLayoutImplInternals::computeNbrGhosts(uint32 ghostParameter)
{
    nbrPrimalGhosts_ = static_cast<uint32> ( std::floor( ghostParameter/2.    ) ) ;
    nbrDualGhosts_   = static_cast<uint32> ( std::floor((ghostParameter +1)/2.) ) ;
}


double GridLayoutImplInternals::inverseSpatialStep( Direction direction ) const noexcept
{
    uint32 iDir = static_cast<uint32>( direction ) ;

    return odxdydz_[iDir] ;
}


uint32 GridLayoutImplInternals::nbrPaddingCells( Direction direction ) const noexcept
{
    uint32 iDir = static_cast<uint32>( direction ) ;

    return nbrPaddingCells_[iDir] ;
}


uint32 GridLayoutImplInternals::nbrPhysicalCells( Direction direction ) const noexcept
{
    uint32 iDir = static_cast<uint32>( direction ) ;

    return nbrPhysicalCells_[iDir] ;
}


uint32 GridLayoutImplInternals::cellIndexAtMin( QtyCentering centering,
                                                Direction direction ) const
{

    uint32 cellIndex = nbrPaddingCells( direction ) + nbrGhosts( centering );

    return cellIndex ;
}


uint32 GridLayoutImplInternals::cellIndexAtMax( QtyCentering centering,
                                                Direction direction ) const
{
    uint32 cellIndex = cellIndexAtMin(centering, direction)
                     + nbrPhysicalCells( direction )
                     - isDual(centering) ;

    return cellIndex ;
}


uint32 GridLayoutImplInternals::ghostCellIndexAtMax( QtyCentering centering,
                                                     Direction direction ) const
{
    uint32 cellIndex = cellIndexAtMax( centering, direction ) + nbrGhosts( centering );

    return cellIndex ;
}

uint32 GridLayoutImplInternals::nbrGhosts( QtyCentering centering ) const noexcept
{
    uint32 nbrGhosts = nbrPrimalGhosts_ ;

    if( centering == QtyCentering::dual )
    {
        nbrGhosts = nbrDualGhosts_;
    }

    return nbrGhosts ;
}


uint32 GridLayoutImplInternals::isDual( QtyCentering centering ) const noexcept
{
    uint32 offset = 0 ;

    if( centering == QtyCentering::dual )
    {
        offset = 1 ;
    }

    return offset ;
}




