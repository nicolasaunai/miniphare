
#include "pusher/modifiedboris.h"



/**
 * @brief Modified Boris pusher proposed by Kunz
 * @param particle
 * we get position and velocity at time tn
 * @param dt
 * @param m
 * @param q
 * @param E is given at tn+1/2
 * @param B is given at tn+1/2
 */
void ModifiedBoris::move1D(Particle & particle,
                           double dt, double m, double q,
                           VecField const & E ,
                           VecField const & B )
{
    // the position xn (at time tn) is given by
    // particle.icell[0]*dx_+ particle.delta[0]

    // we keep only delta_xn
    float delta_xn = particle.delta[0] ;

    // time decentering





}

void ModifiedBoris::move2D(Particle & particle,
                           double dt, double m, double q,
                           VecField const & E ,
                           VecField const & B )
{

}


void ModifiedBoris::move3D(Particle & particle,
                           double dt, double m, double q,
                           VecField const & E ,
                           VecField const & B )
{

}


