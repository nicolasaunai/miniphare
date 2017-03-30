#ifndef MLMDPARTICLEINITIALIZER_H
#define MLMDPARTICLEINITIALIZER_H

#include <memory>

#include "Initializer/particleinitializer.h"
#include "AMR/particleselector.h"
#include "Plasmas/species.h"
#include "Splitting/splittingstrategy.h"

#include "utilityphare.h"



class MLMDParticleInitializer : public ParticleInitializer
{

private:
    Species const& particleSource_;
    ParticleSelector const & selector_;
    SplittingStrategy const & strategy_;

public:
    MLMDParticleInitializer( Species const& particleSource,
                             ParticleSelector const & selector,
                             SplittingStrategy const & strategy )
                        : particleSource_{particleSource},
                          selector_{selector},
                          strategy_{strategy} {}

    virtual ~MLMDParticleInitializer() = default;

    virtual void loadParticles(std::vector<Particle>& particles) const override;
};




#endif // MLMDPARTICLEINITIALIZER_H
