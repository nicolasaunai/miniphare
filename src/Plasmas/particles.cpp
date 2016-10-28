#include "particles.h"



Particle::Particle(double weightRel, std::array<double, 3> pos,
                   std::array<double, 3>  vel)
{
    weight = weightRel ;

    r = pos ;
    v = vel ;
}

