#include "periodicdomainboundary.h"




void PeriodicDomainBoundary::applyMagneticBC(VecField& B, GridLayout const& layout) const
{
    for (Field& component : B.components())
    {
        makeFieldPeriodic_(component, layout);
    }
}



void PeriodicDomainBoundary::applyElectricBC(VecField& E, GridLayout const& layout) const
{
    for (Field& component : E.components())
    {
        makeFieldPeriodic_(component, layout);
    }
}


void PeriodicDomainBoundary::applyCurrentBC(VecField& J, GridLayout const& layout) const
{
    for (Field& component : J.components())
    {
        makeFieldPeriodic_(component, layout);
    }
}





void PeriodicDomainBoundary::applyDensityBC(Field& Ni, GridLayout const& layout) const
{
    makeFieldPeriodic_(Ni, layout);
}






void PeriodicDomainBoundary::applyBulkBC(VecField& Vi, GridLayout const& layout) const
{
    for (Field& component : Vi.components())
    {
        makeFieldPeriodic_(component, layout);
    }
}





void PeriodicDomainBoundary::makeFieldPeriodic_(Field& field, GridLayout const& layout) const
{
    switch (layout.nbDimensions())
    {
        case 1:
        makeFieldPeriodic1D_(field, layout);
        break;

        case 2:
        makeFieldPeriodic2D_(field, layout);
        break;

        case 3:
        makeFieldPeriodic3D_(field, layout);
        break;
    }
}


void PeriodicDomainBoundary::makeFieldPeriodic1D_(Field& field, GridLayout const& layout) const
{

    /* periodic boundary condition is special in the sense that they
       are applied to 2 edges in the same direction instead of one.
       rather than looping over one edge only, we loop over */
#if 1
    if( edge_ == Edge::Xmin)
    {
        /* we apply periodic BCs only to those
               which are DUAL. Primal components should have been
               **calculated** correctly
            */
        auto centering   = layout.fieldCentering( field, Direction::X) ;
        uint32 physStart = layout.physicalStartIndex( field, Direction::X) ;
        uint32 physEnd   = layout.physicalEndIndex  ( field, Direction::X) ;
        uint32 nbrGhosts = layout.nbrGhostCells( centering ) ;

        if (centering == QtyCentering::dual)
        {
            for( uint32 ig=1 ; ig<nbrGhosts+1 ; ++ig )
            {
                field( physStart- ig ) = field( physEnd   - ig ) ;
                field( physEnd  + ig ) = field( physStart + ig ) ;
            }
        }

        else
        {
            for( uint32 ig=0 ; ig<nbrGhosts ; ++ig )
            {
                field( physStart- ig ) = field( physEnd   - ig ) ;
                field( physEnd  + ig ) = field( physStart + ig ) ;
            }
        }
    } // end if at Min boundary
#endif
}






void PeriodicDomainBoundary::makeFieldPeriodic2D_(Field& field, GridLayout const& layout) const
{
    throw std::runtime_error("Not Implemented");
}


void PeriodicDomainBoundary::makeFieldPeriodic3D_(Field& field, GridLayout const& layout) const
{
    throw std::runtime_error("Not Implemented");
}










