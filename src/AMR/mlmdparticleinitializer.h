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

    GridLayout coarseLayout_ ;
    GridLayout refinedLayout_;

    const uint32 refinementRatio_ ;

public:
    MLMDParticleInitializer( Species const& particleSource,
                             std::unique_ptr<ParticleSelector> selector,
                             std::unique_ptr<SplittingStrategy> strategy,
                             GridLayout const & coarseLayout,
                             GridLayout const & refinedLayout,
                             uint32 refinementRatio )
                        : particleSource_{particleSource},
                          selector_{ std::move(selector) },
                          strategy_{ std::move(strategy) },
                          coarseLayout_{coarseLayout},
                          refinedLayout_{refinedLayout},
                          refinementRatio_{refinementRatio} {}

    virtual ~MLMDParticleInitializer() = default;

    virtual void loadParticles(std::vector<Particle>& particles) const override;
};




#endif // MLMDPARTICLEINITIALIZER_H
