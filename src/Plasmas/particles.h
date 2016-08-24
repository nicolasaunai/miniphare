#ifndef PARTICLES_H
#define PARTICLES_H

#include <vector>


class Particle
{
public:
    double r[3];
    double v[3];
};



class Particles
{
private:

    std::vector<Particle> partArr;


public:
    Particles();
    virtual ~Particles();

    inline std::vector<Particle>& getParticleArray()
    { return partArr;}
};

#endif // PARTICLES_H

