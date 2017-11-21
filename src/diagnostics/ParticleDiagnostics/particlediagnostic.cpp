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
    if (strategy_ == nullptr)
        throw std::runtime_error("ParticleDiagnostic Error - No compute Strategy");


    auto const& patchTable = hierarchy.patchTable();
    for (auto const& level : patchTable)
    {
        for (auto const& patch : level)
        {
            packs_.push_back(strategy_->compute(*patch));
        }
    }
}


void ParticleDiagnostic::flushPacks()
{
    std::vector<ParticlePack> tmp;
    std::swap(tmp, packs_);
}


#endif // PARTICLEDIAGNOSTIC_CPP
