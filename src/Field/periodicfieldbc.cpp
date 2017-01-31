
#include "Field/periodicfieldbc.h"



void PeriodicFieldBC::applyElectricBC( VecField & E )
{
    uint32 idirX = static_cast<uint32>(Direction::X) ;
    uint32 idirY = static_cast<uint32>(Direction::Y) ;
    uint32 idirZ = static_cast<uint32>(Direction::Z) ;

    Field & Ex = E.component(idirX) ;
    Field & Ey = E.component(idirY) ;
    Field & Ez = E.component(idirZ) ;

    std::vector<std::reference_wrapper<Field>> Exyz = {Ex, Ey, Ez} ;

    makeFieldPeriodic_( Exyz ) ;

}



void PeriodicFieldBC::applyMagneticBC( VecField & B )
{
    uint32 idirX = static_cast<uint32>(Direction::X) ;
    uint32 idirY = static_cast<uint32>(Direction::Y) ;
    uint32 idirZ = static_cast<uint32>(Direction::Z) ;

    Field & Bx = B.component(idirX) ;
    Field & By = B.component(idirY) ;
    Field & Bz = B.component(idirZ) ;

    std::vector<std::reference_wrapper<Field>> Bxyz = {Bx, By, Bz} ;

    makeFieldPeriodic_( Bxyz ) ;

}



void PeriodicFieldBC::applyMomentsBC( VecField & moment )
{
    uint32 idirX = static_cast<uint32>(Direction::X) ;
    uint32 idirY = static_cast<uint32>(Direction::Y) ;
    uint32 idirZ = static_cast<uint32>(Direction::Z) ;

    Field & Mx = moment.component(idirX) ;
    Field & My = moment.component(idirY) ;
    Field & Mz = moment.component(idirZ) ;

    std::vector<std::reference_wrapper<Field>> Mxyz = {Mx, My, Mz} ;

    // WARNING
    // we shall apply boundary conditions to the computed
    // Jx, Jy, Jz fields
    // but Jx, Jy, Jz are not members of enum class HybridQuantity
    // this must be clarified
//    makeFieldPeriodic_( Mxyz ) ;

}



void PeriodicFieldBC::makeFieldPeriodic_( std::vector<std::reference_wrapper<Field>> Fxyz )
{

    uint32  computedEdge = EDGE_MAX_ ;

    if( edge_ == Edge::Xmin || edge_ == Edge::Ymin || edge_ == Edge::Zmin )
    {
        computedEdge = EDGE_MIN_ ;
    }

    // We apply periodic boundary conditions only once
    // when we handle an EDGE_MIN_
    if( computedEdge == EDGE_MIN_ )
    {
        for( uint32 ifield=0 ; ifield<Fxyz.size() ; ++ifield )
        {
            Field & field = Fxyz[ifield] ;

            uint32 physStart = layout_.physicalStartIndex( field, direction_ ) ;
            uint32 physEnd   = layout_.physicalEndIndex  ( field, direction_ ) ;

            auto centering = layout_.fieldCentering( field, Direction::X ) ;

            uint32 nbrGhosts = layout_.nbrGhostCells( centering ) ;

            for( uint32 ig=1 ; ig<nbrGhosts+1 ; ++ig )
            {
                field( physStart-ig ) = field( physEnd-ig ) ;

                field( physEnd+ig ) = field( physStart+ig ) ;
            }

        }
    }

}



