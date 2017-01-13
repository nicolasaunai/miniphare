#ifndef INTERPOLATEFIELDS_H
#define INTERPOLATEFIELDS_H

#include <vector>

#include "grid/gridlayout.h"
#include "Plasmas/particles.h"
#include "Interpolator/interpolator.h"


void compute1DFieldsAtParticles( Interpolator & interp,
                                 Particle & part,
                                 GridLayout const & layout,
                                 VecField const & E , VecField const & B );


#endif // INTERPOLATEFIELDS_H
