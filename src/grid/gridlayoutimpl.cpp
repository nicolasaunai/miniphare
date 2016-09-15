
#include "gridlayoutimpl.h"

#include "hybridenums.h"

#include <cmath>
#include <stdexcept>


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


QtyCentering GridLayoutImplInternals::derivedLayout(HybridQuantity qty, Direction dir) const
{

    uint32 iField = static_cast<uint32>( qty ) ;
    uint32 idir   = static_cast<uint32>( dir ) ;

    QtyCentering newLayout = changeLayout( hybridQtyCentering_[iField][idir] ) ;

    return newLayout ;
}


QtyCentering GridLayoutImplInternals::changeLayout(QtyCentering layout ) const
{
    QtyCentering newLayout = QtyCentering::primal ;

    if( layout == QtyCentering::primal )
    {
        newLayout = QtyCentering::dual ;
    }

    return newLayout ;
}

/**
 * @brief GridLayoutImplInternals::computeOffsets
 * This method computes the number of ghost cells for fields.
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
    nbrPrimalGhosts_ = static_cast<uint32> ( std::floor(ghostParameter/2.) )      ;
    nbrDualGhostsLeft_     = static_cast<uint32> ( std::floor((ghostParameter -1)/2.) ) ;
    nbrDualGhostsRight_    = static_cast<uint32> ( std::floor((ghostParameter +1)/2.) ) ;
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

    uint32 cellIndex = nbrPaddingCells( direction ) + nbrGhostAtMin( centering );

    return cellIndex ;
}


uint32 GridLayoutImplInternals::cellIndexAtMax( QtyCentering centering,
                                                Direction direction ) const
{
    uint32 cellIndex = cellIndexAtMin(centering, direction) + nbrPhysicalCells( direction ) ;

    return cellIndex ;
}


uint32 GridLayoutImplInternals::ghostCellIndexAtMax( QtyCentering centering,
                                                     Direction direction ) const
{
    uint32 cellIndex = cellIndexAtMax( centering, direction ) + nbrGhostAtMax( centering );

    return cellIndex ;
}

uint32 GridLayoutImplInternals::nbrGhostAtMin( QtyCentering centering ) const noexcept
{
    uint32 nbrGhosts = nbrPrimalGhosts_ ;

    if( centering == QtyCentering::dual )
    {
        nbrGhosts = nbrDualGhostsLeft_;
    }

    return nbrGhosts ;
}

uint32 GridLayoutImplInternals::nbrGhostAtMax( QtyCentering centering ) const noexcept
{
    uint32 nbrGhosts = nbrPrimalGhosts_ ;

    if( centering == QtyCentering::dual )
    {
        nbrGhosts = nbrDualGhostsRight_ ;
    }

    return nbrGhosts ;
}



