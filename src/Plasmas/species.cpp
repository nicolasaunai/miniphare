#include "species.h"



Species::Species(GridLayout const& layout, double mass,
                 std::unique_ptr<ParticleInitializer> particleInitializer,
                 std::string const& name)
    : layout_{ layout },
      rho_    { layout_.nx(), layout_.ny(), layout_.nz(), "rho_"   + name },
      flux_{ layout_.nx(), layout_.ny(), layout_.nz(),"bulkVel_"+ name },
      particleArray_{},
      particleInitializer_{ std::move(particleInitializer) } //TODO broken copy
{
    // TODO should check stuff here.
    // like : is particleInitializer OK?
}



void Species::loadParticles()
{
    particleInitializer_->loadParticles(particleArray_);
}
