#ifndef PARTICLES_H
#define PARTICLES_H

#include <array>


class Particle
{
public:
    double weight ;  // variable weight

    std::array<double, 3> r;
    std::array<double, 3> v;

public:
    Particle(double weightRel, std::array<double, 3> pos,
             std::array<double, 3>  vel);

};



#endif // PARTICLES_H

