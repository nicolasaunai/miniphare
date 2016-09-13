#ifndef FLUIDPARTICLEINITIALIZER_H
#define FLUIDPARTICLEINITIALIZER_H


#include "particleinitializer.h"


class FluidParticleInitializer : public ParticleInitializer
{

    using Function = double (*) (double x, double y, double z);

public:

    FluidParticleInitializer();

    Function density;
    Function bulkVelocity;
    Function temperature;

};




#endif // FLUIDPARTICLEINITIALIZER_H
