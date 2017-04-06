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

    std::unique_ptr<ParticleSelector>   selector_;
    std::unique_ptr<SplittingStrategy>  strategy_;

public:
    MLMDParticleInitializer( Species const& particleSource,
                             std::unique_ptr<ParticleSelector> selector,
                             std::unique_ptr<SplittingStrategy> strategy )
                        : particleSource_{particleSource},
                          selector_{ std::move(selector) },
                          strategy_{ std::move(strategy) } {}

    virtual ~MLMDParticleInitializer() = default;

    virtual void loadParticles(std::vector<Particle>& particles) const override;
};




#endif // MLMDPARTICLEINITIALIZER_H
