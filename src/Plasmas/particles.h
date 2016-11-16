#ifndef PARTICLES_H
#define PARTICLES_H

#include <array>

#include "types.h"

class Particle
{
public:
    double weight ;  // variable weight

    std::array<int32, 3> icell ;   // node coordinate

    std::array<float, 3> delta ;   // value in [0, 1] in each direction

    std::array<double, 3> v ;      // velocity in each direction

public:
    Particle( double weight,
              std::array<int32, 3> icell,
              std::array<float, 3> delta,
              std::array<double, 3> v   );

};



#endif // PARTICLES_H

