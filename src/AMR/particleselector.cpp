
#include "particleselector.h"



bool isInBox::operator()(Particle const& particle) const
{
    bool isIn = false;

    // TODO: finish implementation of this selector
    double posx = (particle.icell[0] + particle.delta[0]) * dx_  + parentBox_.x0 ;
    double posy = (particle.icell[1] + particle.delta[1]) * dy_  + parentBox_.y0 ;
    double posz = (particle.icell[2] + particle.delta[2]) * dz_  + parentBox_.z0 ;

    //return true if the particle is in the box
    if( posx > newBox_.x0 && posx < newBox_.x1 &&
        posy > newBox_.y0 && posy < newBox_.y1 &&
        posz > newBox_.z0 && posz < newBox_.z1 )
    {
        isIn = true;
    }

    return isIn;
}




