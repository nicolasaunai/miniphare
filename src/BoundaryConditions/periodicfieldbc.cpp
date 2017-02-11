
#include "BoundaryConditions/periodicfieldbc.h"
#include "vecfield/vecfield.h"


void PeriodicFieldBC::applyElectricBC( VecField & E )
{
    makeFieldPeriodic_(E);
}



void PeriodicFieldBC::applyMagneticBC( VecField & B )
{
    makeFieldPeriodic_(B) ;
}


// TODO: this is not a FieldBC method, move to Class MomentBC !
// TODO: revise makeFieldPeriodic_ implementation in future Class PeriodicMomentBC
//void PeriodicFieldBC::applyMomentsBC( VecField & moment )
//{
//    uint32 idirX = static_cast<uint32>(Direction::X) ;
//    uint32 idirY = static_cast<uint32>(Direction::Y) ;
//    uint32 idirZ = static_cast<uint32>(Direction::Z) ;

//    Field & Mx = moment.component(idirX) ;
//    Field & My = moment.component(idirY) ;
//    Field & Mz = moment.component(idirZ) ;

//    std::vector<std::reference_wrapper<Field>> Mxyz = {Mx, My, Mz} ;

    // WARNING
    // we shall apply boundary conditions to the computed
    // Jx, Jy, Jz fields
    // but Jx, Jy, Jz are not members of enum class HybridQuantity
    // this must be clarified
//    makeFieldPeriodic_( Mxyz ) ;

//}



void PeriodicFieldBC::makeFieldPeriodic_(VecField& vecField)
{
    switch (layout_.nbDimensions())
    {
        case 1:
        makeFieldPeriodic1D_(vecField);
        break;

        case 2:
        makeFieldPeriodic2D_(vecField);
        break;

        case 3:
        makeFieldPeriodic3D_(vecField);
        break;
    }
}





void PeriodicFieldBC::makeFieldPeriodic1D_(VecField& vecField)
{

    /* periodic boundary condition is special in the sense that they
       are applied to 2 edges in the same direction instead of one.
       rather than looping over one edge only, we loop over */
    if( edge_ == Edge::Xmin)
    {
#if 0
        for (Field& field : vecField.components())
        {
            /* for each component, we apply periodic BCs only to those
               which are DUAL. Primal components should have been
               **calculated** correctly
            */
            auto centering = layout_.fieldCentering( field, direction_) ;
            if (centering == QtyCentering::dual)
            {
                uint32 physStart = layout_.physicalStartIndex( field, direction_ ) ;
                uint32 physEnd   = layout_.physicalEndIndex  ( field, direction_ ) ;

                /* for each ghost node */
                uint32 nbrGhosts = layout_.nbrGhostCells( centering ) ;
                for( uint32 ig=1 ; ig<nbrGhosts+1 ; ++ig )
                {
                    field( physStart-ig ) = field( physEnd-ig ) ;
                    field( physEnd+ig ) = field( physStart+ig ) ;
                }
            }

        }
        for( uint32 ifield=0 ; ifield<Fxyz.size() ; ++ifield )
        {

        }
#endif
    } // end if at Min boundary
}






void PeriodicFieldBC::makeFieldPeriodic2D_(VecField& vecField)
{
    throw std::runtime_error("Not Implemented");
}


void PeriodicFieldBC::makeFieldPeriodic3D_(VecField& vecField)
{
    throw std::runtime_error("Not Implemented");
}






