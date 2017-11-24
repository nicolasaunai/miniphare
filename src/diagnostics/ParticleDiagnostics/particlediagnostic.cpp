#ifndef PARTICLEDIAGNOSTIC_CPP
#define PARTICLEDIAGNOSTIC_CPP


#include "particlediagnostic.h"



/**
 * @brief compute loops over a Hierarchy and for each Patch call the
 * ParticleDiagnosticStrategy::compute() method. From this method it gets
 * a ParticlePack that is added to the ParticlePack vector.
 */
void ParticleDiagnostic::compute(Hierarchy const& hierarchy)
{
    if (selectorPtr_ == nullptr)
        throw std::runtime_error("ParticleDiagnostic Error - No compute Strategy");


    auto const& patchTable = hierarchy.patchTable();
    for (auto const& level : patchTable)
    {
        for (auto const& patch : level)
        {
            packs_.push_back(compute_(*patch));
        }
    }
}


void ParticleDiagnostic::flushPacks()
{
    std::vector<ParticlePack> tmp;
    std::swap(tmp, packs_);
}


ParticlePack ParticleDiagnostic::compute_(Patch const& patch)
{
    std::cout << "computing RhoSpeciesDiag" << speciesName_ << std::endl;
    ParticlePack pack;

    PatchData const& patchData             = patch.data();
    std::vector<Particle> const& particles = patchData.ions().species(speciesName_).particles();
    GridLayout const& layout               = patch.layout();

    std::vector<Particle> selectedParticles;
    for (Particle const& part : particles)
    {
        if (selectorPtr_->pick(part, layout))
            selectedParticles.push_back(part);
    }


    fillPack_(pack, selectedParticles, layout);

    return pack;
}




/**
 * @brief fillPack_ knows how to extract information from a field and
 * a layout to fill a FieldPack correctly.
 */
void ParticleDiagnostic::fillPack_(ParticlePack& pack, std::vector<Particle> const& particles,
                                   GridLayout const& layout)
{
    pack.gridSpacing[0] = layout.dx();
    pack.gridSpacing[1] = layout.dy();
    pack.gridSpacing[2] = layout.dz();

    pack.origin = layout.origin();

    pack.nbrGhosts   = static_cast<int32>(layout.nbrGhostNodes(QtyCentering::primal));
    pack.nbParticles = particles.size();

    fillDiagData1D_(pack, particles);
}



void ParticleDiagnostic::fillDiagData1D_(ParticlePack& pack, std::vector<Particle> const& particles)
{
    // All selected particles are stored in the ParticlePack
    pack.data = std::move(particles);
}


#endif // PARTICLEDIAGNOSTIC_CPP
