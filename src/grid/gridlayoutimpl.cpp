
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
      odx_{1./dx_}, ody_{1./dy_}, odz_{1./dz_}
{
    computeOffsets( ghostParameter );
}


LayoutType GridLayoutImplInternals::derivedLayout(HybridQuantity qty, Direction dir) const
{

    uint32 iField = static_cast<uint32>( qty ) ;
    uint32 idir   = static_cast<uint32>( dir ) ;

    LayoutType newLayout = changeLayout( hybridQtyCentering_[iField][idir] ) ;

    return newLayout ;
}


LayoutType GridLayoutImplInternals::changeLayout(LayoutType layout ) const
{
    LayoutType newLayout = LayoutType::primal ;

    if( layout == LayoutType::primal )
    {
        newLayout = LayoutType::dual ;
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
void GridLayoutImplInternals::computeOffsets(uint32 ghostParameter)
{
    centeredOffset_ = static_cast<uint32> ( std::floor(ghostParameter/2.) )      ;
    leftOffset_     = static_cast<uint32> ( std::floor((ghostParameter -1)/2.) ) ;
    rightOffset_    = static_cast<uint32> ( std::floor((ghostParameter +1)/2.) ) ;
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


uint32 GridLayoutImplInternals::cellIndexAtMin( LayoutType centering,
                                                Direction direction ) const
{

    uint32 cellIndex = nbrPaddingCells( direction ) + nbrGhostAtMin( centering );

    return cellIndex ;
}


uint32 GridLayoutImplInternals::cellIndexAtMax( LayoutType centering,
                                                Direction direction ) const
{
    uint32 cellIndex = cellIndexAtMin(centering, direction) + nbrPhysicalCells( direction ) ;

    return cellIndex ;
}


uint32 GridLayoutImplInternals::ghostCellIndexAtMax( LayoutType centering,
                                                     Direction direction ) const
{
    uint32 cellIndex = cellIndexAtMax( centering, direction ) + nbrGhostAtMax( centering );

    return cellIndex ;
}

uint32 GridLayoutImplInternals::nbrGhostAtMin( LayoutType centering ) const noexcept
{
    uint32 nbrGhosts = centeredOffset_ ;

    if( centering == LayoutType::dual )
    {
        nbrGhosts = leftOffset_;
    }

    return nbrGhosts ;
}

uint32 GridLayoutImplInternals::nbrGhostAtMax( LayoutType centering ) const noexcept
{
    uint32 nbrGhosts = centeredOffset_ ;

    if( centering == LayoutType::dual )
    {
        nbrGhosts = rightOffset_ ;
    }

    return nbrGhosts ;
}



