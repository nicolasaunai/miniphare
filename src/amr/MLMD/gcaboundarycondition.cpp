
#include "gcaboundarycondition.h"

#include "utilities/particleselector.h"
#include "utilities/particleutilities.h"



GCABoundaryCondition::GCABoundaryCondition(GridLayout const& patchLayout,
                                           GridLayout const& GCALayout)
    : patchLayout_{patchLayout}
    , GCALayout_{GCALayout}
    , incomingParticleBucket_{}
{
}


void GCABoundaryCondition::applyMagneticBC(VecField& B) const
{
    (void)B;
}


void GCABoundaryCondition::applyElectricBC(VecField& E) const
{
    (void)E;
}


void GCABoundaryCondition::applyCurrentBC(VecField& J) const
{
    (void)J;
}



void GCABoundaryCondition::applyDensityBC(Field& Ni) const
{
    (void)Ni;
}



void GCABoundaryCondition::applyFluxBC(Ions& ions) const
{
    (void)ions;
}


void GCABoundaryCondition::applyOutgoingParticleBC(std::vector<Particle>& GCAparticles,
                                                   LeavingParticles const& leavingParticles)
{
    std::vector<uint32> leavingIndexes;

    // build a unique array of leaving particles
    // using data from leavingParticles
    for (uint32 iDim = 0; iDim < GCALayout_.nbDimensions(); ++iDim)
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

    IsInBoxSelector selector{patchLayout_.getBox()};

    // build array containing the specific subset of leaving particles
    // going to the patch
    for (uint32 ipart : leavingIndexes)
    {
        const Particle& part = GCAparticles[ipart];

        if (selector.pick(part, GCALayout_))
        {
            Particle newPart;

            particleChangeLayout(GCALayout_, patchLayout_, part, newPart);

            incomingParticleBucket_.push_back(newPart);
        }
    }

    // Now, we remove all leaving particles from
    // the GCA
    removeParticles(leavingIndexes, GCAparticles);
}



void GCABoundaryCondition::applyIncomingParticleBC(std::vector<Particle>& particles,
                                                   std::string const& pusher, double const& dt,
                                                   std::string const& species) const
{
    (void)particles;
    (void)pusher;
    (void)dt;
    (void)species;
}



GCABoundaryCondition::~GCABoundaryCondition()
{
}
