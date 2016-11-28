
#include <cmath>

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
                           Point const & Epart ,
                           Point const & Bpart )
{
    double dto2 = 0.5*dt ;

    double coef1 = q*dto2/m ;

    // position at time tn
//    double posx = particle.position[0] ;

    double posx = particle.icell[0]*dx_+ static_cast<double>( particle.delta[0] ) ;

    // time decentering position at tn+1/2
    double posx_d = posx + dto2* particle.v[0] ;

    // We now apply the 3 steps of the BORIS PUSHER

    // 1st half push of the electric field
    double velx1 = particle.v[0] + coef1*Epart.x_ ;
    double vely1 = particle.v[1] + coef1*Epart.y_ ;
    double velz1 = particle.v[2] + coef1*Epart.z_ ;


    // preparing variables for magnetic rotation
    double rx = coef1 *Bpart.x_ ;
    double ry = coef1 *Bpart.y_ ;
    double rz = coef1 *Bpart.z_ ;

    double rx2 = rx*rx ;
    double ry2 = ry*ry ;
    double rz2 = rz*rz ;
    double rxry = rx*ry ;
    double rxrz = rx*rz ;
    double ryrz = ry*rz ;

    double invDet = 1./(1. + rx2 + ry2 + rz2 ) ;

    // preparing rotation matrix due to the magnetic field
    // m = invDet*(I + r*r - r x I) - I where x denotes the cross product
    double mxx = 1. + rx2 - ry2 - rz2 ;
    double mxy = 2.*( rxry + rz ) ;
    double mxz = 2.*( rxrz - ry ) ;

    double myx = 2.*( rxry - rz ) ;
    double myy = 1. + ry2 - rx2 - rz2 ;
    double myz = 2.*( ryrz + rx ) ;

    double mzx = 2.*( rxrz + ry ) ;
    double mzy = 2.*( ryrz - rx ) ;
    double mzz = 1. + rz2 - rx2 - ry2 ;

    // magnetic rotation
    double velx2 = ( mxx*velx1 + mxy*vely1 + mxz*velz1 )*invDet ;
    double vely2 = ( myx*velx1 + myy*vely1 + myz*velz1 )*invDet ;
    double velz2 = ( mzx*velx1 + mzy*vely1 + mzz*velz1 )*invDet ;


    // 2nd half push of the electric field
    velx1 = velx2 + coef1*Epart.x_ ;
    vely1 = vely2 + coef1*Epart.y_ ;
    velz1 = velz2 + coef1*Epart.z_ ;

    // we update the position at tn+1
    posx = posx_d + dto2 * velx1 ;


    // Update particle velocity
    particle.v[0] = velx1 ;
    particle.v[1] = vely1 ;
    particle.v[2] = velz1 ;

    // TODO later handle the origin of a patch
    //    particle.position[0] = posx ;
    // get the node coordinate
    particle.icell[0] = static_cast<uint32>( std::floor( posx/dx_ ) ) ;

    // get the delta
    particle.delta[0] = static_cast<float>( posx - particle.icell[0]*dx_ ) ;


}

void ModifiedBoris::move2D(Particle & particle,
                           double dt, double m, double q,
                           Point const & E ,
                           Point const & B )
{

}


void ModifiedBoris::move3D(Particle & particle,
                           double dt, double m, double q,
                           Point const & E ,
                           Point const & B )
{

}


