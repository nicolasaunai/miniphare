#ifndef VIRTUALPARTICLE_H
#define VIRTUALPARTICLE_H

#include <array>

#include "types.h"



/**
 * @brief The VirtualParticle class
 *
 * A virtual particle can have negative node indexes
 *
 */
class VirtualParticle
{
public:
    double weight; // variable weight
    double charge;

    std::array<int32, 3> icell; // node coordinate
    std::array<float, 3> delta; // value in [0, 1] in each direction
    std::array<double, 3> v;    // velocity in each direction

    double Ex, Ey, Ez; // electric field at the particle position
    double Bx, By, Bz; // magnetic field at the particle position

    VirtualParticle() = default;

    VirtualParticle(double weight, double charge, std::array<int32, 3> icell,
                    std::array<float, 3> delta, std::array<double, 3> v);
};


#endif // VIRTUALPARTICLE_H
