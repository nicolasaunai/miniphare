#include "gridlayoutimplinternals.h"

#include <cmath>
#include <stdexcept>




GridLayoutImplInternals::GridLayoutImplInternals(uint32 nbDims, uint32 ghostParameter,
                                                 std::array<uint32,3> nbrCellsXYZ ,
                                                 std::array<double,3> dxdydz      )
    : nbdims_{nbDims},
      nbrCellx_{nbrCellsXYZ[0]}, nbrCelly_{nbrCellsXYZ[1]}, nbrCellz_{nbrCellsXYZ[2]},
      dx_{dxdydz[0]}, dy_{dxdydz[1]}, dz_{dxdydz[2]},
      odxdydz_{ {1./dx_, 1./dy_, 1./dz_} }
{
    computeNbrGhosts( ghostParameter );
}





void GridLayoutImplInternals::initGridUtils( const gridDataT & data )
{
    nbrPhysicalCells_[data.idirX] = nbrCellx_ ;
    nbrPhysicalCells_[data.idirY] = nbrCelly_ ;
    nbrPhysicalCells_[data.idirZ] = nbrCellz_ ;
}






void GridLayoutImplInternals::initPhysicalStart( const gridDataT & data )
{
    uint32 iprimal = static_cast<uint32>(data.primal);
    uint32 idual   = static_cast<uint32>(data.dual);

    physicalStartIndexTable_[iprimal][data.idirX] = nbrGhosts( QtyCentering::primal );
    physicalStartIndexTable_[iprimal][data.idirY] = nbrGhosts( QtyCentering::primal );
    physicalStartIndexTable_[iprimal][data.idirZ] = nbrGhosts( QtyCentering::primal );

    physicalStartIndexTable_[idual][data.idirX] = nbrGhosts( QtyCentering::dual );
    physicalStartIndexTable_[idual][data.idirY] = nbrGhosts( QtyCentering::dual );
    physicalStartIndexTable_[idual][data.idirZ] = nbrGhosts( QtyCentering::dual );
}






void GridLayoutImplInternals::initPhysicalEnd( const gridDataT & data )
{
    uint32 iprimal = static_cast<uint32>(data.primal);
    uint32 idual   = static_cast<uint32>(data.dual);

    physicalEndIndexTable_[iprimal][data.idirX] =   physicalStartIndexTable_[iprimal][data.idirX]
                                                  + nbrPhysicalCells( data.dirX )
                                                  - isDual(data.primal);

    physicalEndIndexTable_[iprimal][data.idirY] =   physicalStartIndexTable_[iprimal][data.idirY]
                                                  + nbrPhysicalCells( data.dirY )
                                                  - isDual(data.primal);

    physicalEndIndexTable_[iprimal][data.idirZ] =   physicalStartIndexTable_[iprimal][data.idirZ]
                                                  + nbrPhysicalCells( data.dirZ )
                                                  - isDual(data.primal);


    physicalEndIndexTable_[idual][data.idirX] =   physicalStartIndexTable_[idual][data.idirX]
                                                  + nbrPhysicalCells( data.dirX )
                                                  - isDual(data.dual);

    physicalEndIndexTable_[idual][data.idirY] =   physicalStartIndexTable_[idual][data.idirY]
                                                  + nbrPhysicalCells( data.dirY )
                                                  - isDual(data.dual);

    physicalEndIndexTable_[idual][data.idirZ] =   physicalStartIndexTable_[idual][data.idirZ]
                                                  + nbrPhysicalCells( data.dirZ )
                                                  - isDual(data.dual);

}


/*
void GridLayoutImplInternals::initGhostStart( const gridDataT & data )
{
    ghostStartIndexTable_[]

}*/


void GridLayoutImplInternals::initGhostEnd( const gridDataT & data )
{
    uint32 iprimal = static_cast<uint32>(data.primal);
    uint32 idual   = static_cast<uint32>(data.dual);

    ghostEndIndexTable_[iprimal][data.idirX] =  physicalEndIndexTable_[iprimal][data.idirX]
                                              + nbrGhosts(data.primal);

    ghostEndIndexTable_[iprimal][data.idirY] =  physicalEndIndexTable_[iprimal][data.idirY]
                                              + nbrGhosts(data.primal);

    ghostEndIndexTable_[iprimal][data.idirZ] =  physicalEndIndexTable_[iprimal][data.idirZ]
                                              + nbrGhosts(data.primal);

    ghostEndIndexTable_[idual][data.idirX]   =  physicalEndIndexTable_[idual][data.idirX]
                                              + nbrGhosts(data.dual);

    ghostEndIndexTable_[idual][data.idirY]   =  physicalEndIndexTable_[idual][data.idirY]
                                              + nbrGhosts(data.dual);

    ghostEndIndexTable_[idual][data.idirZ]   =  physicalEndIndexTable_[idual][data.idirZ]
                                              + nbrGhosts(data.dual);




}




QtyCentering GridLayoutImplInternals::derivedCentering(HybridQuantity qty, Direction dir) const
{

    uint32 iField = static_cast<uint32>( qty ) ;
    uint32 idir   = static_cast<uint32>( dir ) ;

    QtyCentering newLayout = changeCentering( hybridQtyCentering_[iField][idir] ) ;

    return newLayout ;
}






std::array<uint32, NBR_COMPO> GridLayoutImplInternals::nodeNbrFromCentering_(
        std::array<QtyCentering, NBR_COMPO> const & qtyCenterings ) const
{
    gridDataT data{} ;

    uint32 nx =  nbrPhysicalCells( Direction::X ) + 1
               + 2*nbrGhosts( qtyCenterings[data.idirX] ) ;

    uint32 ny =  nbrPhysicalCells( Direction::Y ) + 1
               + 2*nbrGhosts( qtyCenterings[data.idirY] ) ;

    uint32 nz =  nbrPhysicalCells( Direction::Z ) + 1
               + 2*nbrGhosts( qtyCenterings[data.idirZ] ) ;

    return { {nx, ny, nz} } ;
}










AllocSizeT GridLayoutImplInternals::allocSize_( HybridQuantity qty ) const
{
    gridDataT data{} ;

    uint32 iQty = static_cast<uint32>(qty) ;

    std::array<QtyCentering, NBR_COMPO>
            qtyCenterings{ {hybridQtyCentering_[iQty][data.idirX],
                            hybridQtyCentering_[iQty][data.idirY],
                            hybridQtyCentering_[iQty][data.idirZ]} } ;

    auto allocSizes = nodeNbrFromCentering_( qtyCenterings ) ;

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

    auto allocSizes = nodeNbrFromCentering_( qtyCenterings ) ;

    return AllocSizeT( allocSizes[0], allocSizes[1], allocSizes[2] );
}






uint32 GridLayoutImplInternals::physicalStartIndex_(QtyCentering centering, Direction direction) const
{
    uint32 icentering = static_cast<uint32>(centering);
    uint32 iDir = static_cast<uint32>(direction);
    return physicalStartIndexTable_[icentering][iDir];
}





uint32 GridLayoutImplInternals::physicalStartIndex_(Field const& field, Direction direction) const
{
    uint32 iQty = static_cast<uint32>(field.hybridQty());
    uint32 iDir = static_cast<uint32>(direction);

    uint32 iCentering = static_cast<uint32>(hybridQtyCentering_[iQty][iDir]);

    return physicalStartIndexTable_[iCentering][iDir];
}






uint32 GridLayoutImplInternals::physicalEndIndex_(QtyCentering centering, Direction direction) const
{
    uint32 icentering = static_cast<uint32>(centering);
    uint32 iDir = static_cast<uint32>(direction);
    return physicalEndIndexTable_[icentering][iDir];
}





uint32 GridLayoutImplInternals::physicalEndIndex_(Field const& field, Direction direction) const
{
    uint32 iQty = static_cast<uint32>(field.hybridQty());
    uint32 iDir = static_cast<uint32>(direction);

    uint32 iCentering = static_cast<uint32>(hybridQtyCentering_[iQty][iDir]);

    return physicalEndIndexTable_[iCentering][iDir];
}








uint32 GridLayoutImplInternals::ghostStartIndex_(Field const& field, Direction direction) const
{
    // ghostStartIndex is always the first node
    return 0;
}






uint32 GridLayoutImplInternals::ghostEndIndex_(Field const& field, Direction direction) const
{
    uint32 iQty = static_cast<uint32>(field.hybridQty());
    uint32 iDir = static_cast<uint32>(direction);
    return ghostEndIndexTable_[iQty][iDir];
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
    uint32 idirX   = static_cast<uint32>(Direction::X) ;
    uint32 idirY   = static_cast<uint32>(Direction::Y) ;
    uint32 idirZ   = static_cast<uint32>(Direction::Z) ;
    uint32 iprimal = static_cast<uint32>(QtyCentering::primal);

    uint32 ixStart = physicalEndIndexTable_[iprimal][idirX];
    uint32 iyStart = physicalEndIndexTable_[iprimal][idirY];
    uint32 izStart = physicalEndIndexTable_[iprimal][idirZ];


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
    uint32 idirX   = static_cast<uint32>(Direction::X) ;
    uint32 idirY   = static_cast<uint32>(Direction::Y) ;
    uint32 idirZ   = static_cast<uint32>(Direction::Z) ;
    uint32 iprimal = static_cast<uint32>(QtyCentering::primal);

    uint32 ixStart = physicalEndIndexTable_[iprimal][idirX];
    uint32 iyStart = physicalEndIndexTable_[iprimal][idirY];
    uint32 izStart = physicalEndIndexTable_[iprimal][idirZ];

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




uint32 GridLayoutImplInternals::nbrPhysicalCells( Direction direction ) const noexcept
{
    uint32 iDir = static_cast<uint32>( direction ) ;

    return nbrPhysicalCells_[iDir] ;
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



