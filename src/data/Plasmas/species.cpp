#include "species.h"
#include "data/grid/gridlayout.h"


Species::Species(GridLayout const& layout, double mass,
                 std::unique_ptr<ParticleInitializer> particleInitializer, std::string const& name)
    : mass_{mass}
    , name_{name}
    , rho_{layout.allocSize(HybridQuantity::rho), HybridQuantity::rho, "_rhoTot"}
    , flux_{layout.allocSize(HybridQuantity::V),
            layout.allocSize(HybridQuantity::V),
            layout.allocSize(HybridQuantity::V),
            {{HybridQuantity::V, HybridQuantity::V, HybridQuantity::V}},
            "_fluxSpecies"}
    , particleArray_{}
    , particleInitializer_{std::move(particleInitializer)} // TODO broken copy
{
    // TODO should check stuff here.
    // like : is particleInitializer OK?
}



void Species::loadParticles()
{
    particleInitializer_->loadParticles(particleArray_);
}
