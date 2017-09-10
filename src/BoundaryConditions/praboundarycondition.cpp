
#include "praboundarycondition.h"

#include "AMR/particleselector.h"
#include "AMR/particleutilities.h"



PRABoundaryCondition::PRABoundaryCondition(GridLayout const& patchLayout,
                                           GridLayout const& PRALayout)
    : patchLayout_{patchLayout}
    , PRALayout_{PRALayout}
    , incomingParticleBucket_{}
{
}


void PRABoundaryCondition::applyMagneticBC(VecField& B) const
{
    (void)B;
}


void PRABoundaryCondition::applyElectricBC(VecField& E) const
{
    (void)E;
}


void PRABoundaryCondition::applyCurrentBC(VecField& J) const
{
    (void)J;
}



void PRABoundaryCondition::applyDensityBC(Field& Ni) const
{
    (void)Ni;
}


void PRABoundaryCondition::applyBulkBC(VecField& Vi) const
{
    (void)Vi;
}



void PRABoundaryCondition::applyOutgoingParticleBC(std::vector<Particle>& PRAparticles,
                                                   LeavingParticles const& leavingParticles)
{
    std::vector<uint32> leavingIndexes;

    // build a unique array of leaving particles
    // using data from leavingParticles
    for (uint32 iDim = 0; iDim < PRALayout_.nbDimensions(); ++iDim)
    {
        auto const& indexesAtMin = leavingParticles.indexesAtMin()[iDim];
        uint32 sizeAtMin         = static_cast<uint32>(indexesAtMin.size());
        for (uint32 ipart = 0; ipart < sizeAtMin; ++ipart)
        {
            leavingIndexes.push_back(indexesAtMin[ipart]);
        }
        auto const& indexesAtMax = leavingParticles.indexesAtMax()[iDim];
        uint32 sizeAtMax         = static_cast<uint32>(indexesAtMax.size());
        for (uint32 ipart = 0; ipart < sizeAtMax; ++ipart)
        {
            leavingIndexes.push_back(indexesAtMax[ipart]);
        }
    }

    IsInBoxSelector selector{PRALayout_, patchLayout_.getBox()};

    // build array containing the specific subset of leaving particles
    // going to the patch
    for (uint32 ipart : leavingIndexes)
    {
        const Particle& part = PRAparticles[ipart];

        /*        if (!isInSpecifiedBox(PRALayout_, part, PRALayout_))
                {
                    if (isInSpecifiedBox(PRALayout_, part, patchLayout_))
                    {*/

        if (selector.pick(part))
        {
            Particle newPart;

            particleChangeLayout(PRALayout_, patchLayout_, part, newPart);

            incomingParticleBucket_.push_back(newPart);
        }
    }

    // Now, we remove all leaving particles from
    // the PRA
    removeParticles(leavingIndexes, PRAparticles);
}



void PRABoundaryCondition::applyIncomingParticleBC(Ions& ions, std::string const& pusher,
                                                   double const& dt) const
{
    (void)ions;
    (void)pusher;
    (void)dt;
}



PRABoundaryCondition::~PRABoundaryCondition()
{
}
