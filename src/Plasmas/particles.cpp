#include "particles.h"




//Particle::Particle( double weight, double charge,
//          std::array<uint32, 3> icell,
//          std::array<float, 3> delta,
//          std::array<double, 3> v   )
//{
//    this->weight = weight ;

//    this->charge = charge ;

//    this->icell = icell ;

//    this->delta = delta ;

//    this->v = v ;
//}


Particle::Particle( double weight, double charge,
          std::array<double, 3> position,
          std::array<double, 3> v   )
{
    this->weight = weight ;

    this->charge = charge ;

    this->position = position ;

    this->v = v ;
}
