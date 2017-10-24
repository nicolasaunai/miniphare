#ifndef PARTICLES_H
#define PARTICLES_H

#include <array>

#include "utilities/types.h"



class Particle
{
public:
    double weight; // variable weight
    double charge;

    std::array<int32, 3> icell; // node coordinate
    std::array<float, 3> delta; // value in [0, 1] in each direction
    std::array<double, 3> v;    // velocity in each direction

    double Ex, Ey, Ez; // electric field at the particle position
    double Bx, By, Bz; // magnetic field at the particle position

    Particle() = default;

    Particle(double weight, double charge, std::array<int32, 3> icell, std::array<float, 3> delta,
             std::array<double, 3> v)
        : weight{weight}
        , charge{charge}
        , icell{icell}
        , delta{delta}
        , v{v}
    {
        Ex = 0.;
        Ey = 0.;
        Ez = 0.;
        Bx = 0.;
        By = 0.;
        Bz = 0.;
    }
};



#endif // PARTICLES_H
