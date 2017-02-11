#include "periodicdomainboundary.h"




void PeriodicDomainBoundary::applyMagneticBC(VecField& B, GridLayout const& layout) const
{
    makeFieldPeriodic_(B, layout);
}



void PeriodicDomainBoundary::applyElectricBC(VecField& E, GridLayout const& layout) const
{
    makeFieldPeriodic_(E, layout);
}


void PeriodicDomainBoundary::applyCurrentBC(VecField& J, GridLayout const& layout) const
{
    makeFieldPeriodic_(J, layout);
}




void PeriodicDomainBoundary::makeFieldPeriodic_(VecField& vecField, GridLayout const& layout) const
{
    switch (layout.nbDimensions())
    {
        case 1:
        makeFieldPeriodic1D_(vecField, layout);
        break;

        case 2:
        makeFieldPeriodic2D_(vecField, layout);
        break;

        case 3:
        makeFieldPeriodic3D_(vecField, layout);
        break;
    }
}






void PeriodicDomainBoundary::makeFieldPeriodic1D_(VecField& vecField, GridLayout const& layout) const
{

    /* periodic boundary condition is special in the sense that they
       are applied to 2 edges in the same direction instead of one.
       rather than looping over one edge only, we loop over */
#if 0
    if( edge_ == Edge::Xmin)
    {
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
    } // end if at Min boundary
#endif
}






void PeriodicDomainBoundary::makeFieldPeriodic2D_(VecField& vecField, GridLayout const& layout) const
{
    throw std::runtime_error("Not Implemented");
}


void PeriodicDomainBoundary::makeFieldPeriodic3D_(VecField& vecField, GridLayout const& layout) const
{
    throw std::runtime_error("Not Implemented");
}



