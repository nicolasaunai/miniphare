#include "periodicdomainboundary.h"



/* ----------------------------------------------------------------------------
 *
 *                        FIELD BOUNDARY CONDITIONS
 *
   ---------------------------------------------------------------------------- */
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
        case 1: makeFieldPeriodic1D_(vecField, layout); break;

        case 2: makeFieldPeriodic2D_(vecField, layout); break;

        case 3: makeFieldPeriodic3D_(vecField, layout); break;
    }
}


void PeriodicDomainBoundary::makeFieldPeriodic1D_(VecField& vecField,
                                                  GridLayout const& layout) const
{
    /* periodic boundary condition is special in the sense that they
       are applied to 2 edges in the same direction instead of one.
       rather than looping over one edge only, we loop over */
    if (edge_ == Edge::Xmin)
    {
        for (Field& field : vecField.components())
        {
            /* for each component, we apply periodic BCs only to those
               which are DUAL. Primal components should have been
               **calculated** correctly
            */
            auto centering   = layout.fieldCentering(field, Direction::X);
            uint32 nbrGhosts = layout.nbrGhostNodes(centering);
            uint32 physStart = layout.physicalStartIndex(field, Direction::X);
            uint32 physEnd   = layout.physicalEndIndex(field, Direction::X);

            if (centering == QtyCentering::dual)
            {
                for (uint32 ig = 1; ig <= nbrGhosts; ++ig)
                {
                    field(physStart - ig) = field(physEnd - ig + 1);
                    field(physEnd + ig)   = field(physStart + ig - 1);
                }
            }

            else if (centering == QtyCentering::primal)
            {
                for (uint32 ig = 1; ig <= nbrGhosts; ++ig)
                {
                    field(physStart - ig) = field(physEnd - ig);
                    field(physEnd + ig)   = field(physStart + ig);
                }
            }
        }
    } // end if at Min boundary
}




void PeriodicDomainBoundary::makeFieldPeriodic2D_(VecField& vecField,
                                                  GridLayout const& layout) const
{
    throw std::runtime_error("Not Implemented");
}


void PeriodicDomainBoundary::makeFieldPeriodic3D_(VecField& vecField,
                                                  GridLayout const& layout) const
{
    throw std::runtime_error("Not Implemented");
}



/* ----------------------------------------------------------------------------
 *
 *                        MOMENTS BOUNDARY CONDITIONS
 *
   ---------------------------------------------------------------------------- */



void PeriodicDomainBoundary::applyDensityBC(Field& Ni, GridLayout const& layout) const
{
    makeMomentPeriodic_(Ni, layout);
}



void PeriodicDomainBoundary::applyFluxBC(Ions& ions, GridLayout const& layout) const
{
    for (uint32 ispe = 0; ispe < ions.nbrSpecies(); ++ispe)
    {
        Species& species = ions.species(ispe);

        for (Field& component : species.flux().components())
        {
            makeMomentPeriodic_(component, layout);
        }
    }
}



// moments have to have their own method (makeMomentPeriodic_) and cannot use
// makeFieldPeriodic_ because the treatment of moments is slightly different from
// the one done for fields. They are defined on primal, all components need to be
// treated, and that includes physicalStartIndex and physicalEndIndex.
void PeriodicDomainBoundary::makeMomentPeriodic_(Field& moment, GridLayout const& layout) const
{
    switch (layout.nbDimensions())
    {
        case 1: makeMomentPeriodic1D_(moment, layout); break;

        case 2: makeMomentPeriodic2D_(moment, layout); break;

        case 3: makeMomentPeriodic3D_(moment, layout); break;
    }
}




void PeriodicDomainBoundary::makeMomentPeriodic1D_(Field& moment, GridLayout const& layout) const
{
    // apply boundary conditions only once
    if (edge_ == Edge::Xmin)
    {
        uint32 phyStart  = layout.physicalStartIndex(moment, Direction::X);
        uint32 physEnd   = layout.physicalEndIndex(moment, Direction::X);
        uint32 nbrGhosts = layout.nbrGhostNodes(QtyCentering::primal);

        // need to deal with points on border separately
        // because in the following loop
        // ig=0 would do the reduction twice
        double tmp = moment(phyStart);
        moment(phyStart) += moment(physEnd);
        moment(physEnd) += tmp;

        // now loop on ghost nodes
        for (uint32 ig = 1; ig <= nbrGhosts; ++ig)
        {
            tmp = moment(phyStart - ig);
            moment(phyStart - ig) += moment(physEnd - ig);
            moment(physEnd - ig) += tmp;

            tmp = moment(physEnd + ig);
            moment(physEnd + ig) += moment(phyStart + ig);
            moment(phyStart + ig) += tmp;
        }
    }
}




void PeriodicDomainBoundary::makeMomentPeriodic2D_(Field& moment, GridLayout const& layout) const
{
    throw std::runtime_error("Not Implemented");
}


void PeriodicDomainBoundary::makeMomentPeriodic3D_(Field& moment, GridLayout const& layout) const
{
    throw std::runtime_error("Not Implemented");
}




/* ----------------------------------------------------------------------------
 *
 *                        PARTICLE BOUNDARY CONDITIONS
 *
   ---------------------------------------------------------------------------- */




void makeParticlesPeriodic(std::vector<Particle>& particleArray,
                           LeavingParticles const& leavingParticles)
{
    // loop on dimensions of leavingParticles.particleIndicesAtMin/Max
    uint32 nbDims = leavingParticles.particleIndicesAtMax.size();

    for (uint32 dim = 0; dim < nbDims; ++dim)
    {
        std::vector<int32> const& leavingAtMin = leavingParticles.particleIndicesAtMin[dim];
        std::vector<int32> const& leavingAtMax = leavingParticles.particleIndicesAtMax[dim];

        // loop on all particles leaving at Min
        for (auto index : leavingAtMin)
        {
            particleArray[index].icell[dim] = leavingParticles.startEndIndices[dim].lastCellIndex;
        }

        // now at max
        for (auto index : leavingAtMax)
        {
            particleArray[index].icell[dim] = leavingParticles.startEndIndices[dim].firstCellIndex;
        }
    }
}



void PeriodicDomainBoundary::applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                                     LeavingParticles const& leavingParticles) const
{
    if (edge_ == Edge::Xmin || edge_ == Edge::Ymin || edge_ == Edge::Zmin)
    {
        makeParticlesPeriodic(particleArray, leavingParticles);
    }
}


void PeriodicDomainBoundary::applyIncomingParticleBC(BoundaryCondition& temporaryBC, Pusher& pusher,
                                                     GridLayout const& patchLayout,
                                                     std::vector<Particle>& patchParticles,
                                                     std::string const& species)
{
}
