#ifndef MLMDPARTICLEINITIALIZER_H
#define MLMDPARTICLEINITIALIZER_H

#include <memory>

#include "Initializer/particleinitializer.h"
#include "utility.h"



class MLMDParticleInitializer : public ParticleInitializer
{

private:
    std::unique_ptr<ParticleSelector> selector_;

public:
    MLMDParticleInitializer();
};




#endif // MLMDPARTICLEINITIALIZER_H
