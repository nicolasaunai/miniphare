
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
    double posx = particle.icell[0]*dx_+ static_cast<double>( particle.delta[0] ) ;

    // time decentering position at tn+1/2
    double posx_d = posx + dto2* particle.v[0] ;

    // preparing variables for magnetic rotation
    double rx = coef1 *Bpart.x_ ;
    double ry = coef1 *Bpart.y_ ;
    double rz = coef1 *Bpart.z_ ;

    double invDet = 2./(1. + rx*rx + ry*ry + rz*rz ) ;

    // preparing rotation matrix due to the magnetic field
    // m = invDet*(I + r*r - r x I) - I where x denotes the cross product
    double mxx = invDet*( 1. + rx*rx      ) - 1. ;
    double mxy = invDet*(      rx*ry + rz ) ;
    double mxz = invDet*(      rx*rz - ry ) ;

    double myx = invDet*(      ry*rx - rz ) ;
    double myy = invDet*( 1. + ry*ry      ) - 1. ;
    double myz = invDet*(      ry*rz + rx ) ;

    double mzx = invDet*(      rz*rx + ry ) ;
    double mzy = invDet*(      rz*ry - rx ) ;
    double mzz = invDet*( 1. + rz*rz      ) - 1. ;


    // We now apply the 3 steps of the BORIS PUSHER

    // 1st half push of the electric field
    double velx1 = particle.v[0] + coef1*Epart.x_ ;
    double vely1 = particle.v[1] + coef1*Epart.y_ ;
    double velz1 = particle.v[2] + coef1*Epart.z_ ;

    // magnetic rotation
    double velx2 = mxx*velx1 + mxy*vely1 + mxz*velz1 ;
    double vely2 = myx*velx1 + myy*vely1 + myz*velz1 ;
    double velz2 = mzx*velx1 + mzy*vely1 + mzz*velz1 ;

    // 2nd half push of the electric field
    velx1 = velx2 + coef1*Epart.x_ ;
    vely1 = vely2 + coef1*Epart.y_ ;
    velz1 = velz2 + coef1*Epart.z_ ;

    // we update the position at tn+1
    posx = posx_d + dto2 * velx1 ;

    // Update velocity
    particle.v[0] = velx1 ;
    particle.v[1] = vely1 ;
    particle.v[2] = velz1 ;

    // TODO later handle the origin of a patch
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


