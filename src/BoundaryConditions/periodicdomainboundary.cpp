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
#if 1
    if( edge_ == Edge::Xmin)
    {
        for (Field& field : vecField.components())
        {
            /* for each component, we apply periodic BCs only to those
               which are DUAL. Primal components should have been
               **calculated** correctly
            */
            auto centering = layout.fieldCentering( field, Direction::X) ;
            if (centering == QtyCentering::dual)
            {
                uint32 physStart = layout.physicalStartIndex( field, Direction::X) ;
                uint32 physEnd   = layout.physicalEndIndex  ( field, Direction::X) ;
                uint32 nbrGhosts = layout.nbrGhostCells( centering ) ;

                for( uint32 ig=1 ; ig<nbrGhosts+1 ; ++ig )
                {
                    field( physStart- ig ) = field( physEnd   - ig ) ;
                    field( physEnd  + ig ) = field( physStart + ig ) ;
                }

            }

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









void PeriodicDomainBoundary::makeMomentPeriodic_(Field& moment, GridLayout const& layout) const
{
    switch (layout.nbDimensions())
    {
    case 1:
        makeMomentPeriodic1D_(moment, layout);
        break;

    case 2:
        makeMomentPeriodic2D_(moment, layout);
        break;

    case 3:
        makeMomentPeriodic3D_(moment, layout);
        break;
    }
}


void PeriodicDomainBoundary::applyDensityBC(Field& Ni, GridLayout const& layout) const
{
    makeMomentPeriodic_(Ni, layout);
}




void PeriodicDomainBoundary::applyBulkBC(VecField& Vi, GridLayout const& layout) const
{
    for (Field& component : Vi.components())
    {
        makeMomentPeriodic_(component, layout);
    }
}







void PeriodicDomainBoundary::makeMomentPeriodic1D_(Field& moment, GridLayout const& layout) const
{
    uint32 phyStart = layout.physicalStartIndex(moment, Direction::X);
    uint32 physEnd   = layout.physicalEndIndex  (moment, Direction::X);
    uint32 nbrGhosts = layout.nbrGhostCells(QtyCentering::primal);

    for (uint32 ig=0; ig < nbrGhosts; ++ig)
    {
        moment(phyStart - ig) += moment(physEnd - ig);
        moment(physEnd  + ig) += moment(phyStart + ig);
    }

}





void PeriodicDomainBoundary::makeMomentPeriodic2D_(Field& moment, GridLayout const& layout) const
{

}


void PeriodicDomainBoundary::makeMomentPeriodic3D_(Field& moment, GridLayout const& layout) const
{

}

