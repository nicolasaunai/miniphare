#ifndef MLMDPARTICLEINITIALIZER_H
#define MLMDPARTICLEINITIALIZER_H

#include <memory>

#include "amr/Splitting/splittingstrategy.h"
#include "data/Plasmas/particleinitializer.h"
#include "data/Plasmas/species.h"
#include "utilities/box.h"
#include "utilities/particleselector.h"



class MLMDParticleInitializer : public ParticleInitializer
{
private:
    Species const& particleSource_;

    std::shared_ptr<ParticleSelector> selector_;

    std::unique_ptr<SplittingStrategy> strategy_;

    GridLayout coarseLayout_;
    GridLayout refinedLayout_;


public:
    MLMDParticleInitializer(Species const& particleSource,
                            std::shared_ptr<ParticleSelector> selector,
                            std::unique_ptr<SplittingStrategy> strategy,
                            GridLayout const& coarseLayout, GridLayout const& refinedLayout)
        : particleSource_{particleSource}
        , selector_{selector}
        , strategy_{std::move(strategy)}
        , coarseLayout_{coarseLayout}
        , refinedLayout_{refinedLayout}
    {
    }

    virtual ~MLMDParticleInitializer() = default;

    virtual void loadParticles(std::vector<Particle>& particles) const override;
};




#endif // MLMDPARTICLEINITIALIZER_H
