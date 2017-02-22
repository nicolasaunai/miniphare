#ifndef MLMDPARTICLEINITIALIZER_H
#define MLMDPARTICLEINITIALIZER_H

#include <memory>

#include "Initializer/particleinitializer.h"
#include "Plasmas/species.h"
#include "utilityphare.h"



class MLMDParticleInitializer : public ParticleInitializer
{

private:
    std::unique_ptr<ParticleSelector> selector_;
    Species const& particleSource_;

public:
    MLMDParticleInitializer(Species const& particleSource,
                            std::unique_ptr<ParticleSelector> selector)
                        : selector_{std::move(selector)},
                          particleSource_{particleSource} {}

    virtual void loadParticles(std::vector<Particle>& particles) const override;
    virtual ~MLMDParticleInitializer();
};




#endif // MLMDPARTICLEINITIALIZER_H
