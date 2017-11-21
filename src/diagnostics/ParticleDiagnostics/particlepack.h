#ifndef PARTICLEPACK_H
#define PARTICLEPACK_H

#include <vector>

#include "data/Plasmas/particles.h"

struct ParticlePack
{
    std::vector<Particle> data;
    std::array<double, 3> gridSpacing;
    Point origin;
    int32 nbrGhosts;
    uint64 nbParticles;

    ParticlePack() {}
};

#endif // PARTICLEPACK_H
