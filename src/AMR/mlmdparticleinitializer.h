#ifndef MLMDPARTICLEINITIALIZER_H
#define MLMDPARTICLEINITIALIZER_H

#include <memory>

#include "Initializer/particleinitializer.h"
#include "AMR/particleselector.h"
#include "Plasmas/species.h"

#include "utilityphare.h"



class MLMDParticleInitializer : public ParticleInitializer
{

private:
    Species const& particleSource_;
    ParticleSelector const & selector_;

public:
    MLMDParticleInitializer(Species const& particleSource,
                            ParticleSelector const & selector)
                        : particleSource_{particleSource},
                          selector_{selector} {}

    virtual ~MLMDParticleInitializer() = default;

    virtual void loadParticles(std::vector<Particle>& particles) const override;
};




#endif // MLMDPARTICLEINITIALIZER_H
