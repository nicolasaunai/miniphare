#include "particles.h"




Particle::Particle(double weight, double charge, std::array<uint32, 3> icell,
                   std::array<float, 3> delta, std::array<double, 3> v)
    : weight{weight}
    , charge{charge}
    , icell{icell}
    , delta{delta}
    , v{v}
{
    //    Ex = 0. ; Ey = 0. ; Ez = 0. ;
    //    Bx = 0. ; By = 0. ; Bz = 0. ;
}
