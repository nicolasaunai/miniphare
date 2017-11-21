
#include "particlediagnosticstrategy.h"



void ParticleDiagnosticStrategy::fillDiagData1D_(ParticlePack& pack,
                                                 std::vector<Particle> const& particles)
{
    // All selected particles are stored in the ParticlePack
    pack.data = std::move(particles);
}



/**
 * @brief fillPack_ knows how to extract information from a field and
 * a layout to fill a FieldPack correctly.
 */
void ParticleDiagnosticStrategy::fillPack_(ParticlePack& pack,
                                           std::vector<Particle> const& particles,
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



ParticlePack ParticleDiagnosticStrategy::compute(Patch const& patch)
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
