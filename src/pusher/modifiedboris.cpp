
#include <cmath>

#include "pusher/modifiedboris.h"



#include "helper.h"



/**
 * @brief ModifiedBoris::move1D
 * @param partIn positions and velocities at time tn
 * @param partPred positions and velocities at time tpred
 * @param dt
 * @param m
 * @param E is given at tn+1/2
 * @param B is given at tn+1/2
 */
void ModifiedBoris::move1D( std::vector<Particle> & partIn ,
                            std::vector<Particle> & partOut,
                            double dt, double m,
                            VecField const & E , VecField const & B,
                            Interpolator & interpolator )
{
    double dto2dx = 0.5*dt/dx_ ;

    partOut = partIn ;

    for( uint32 ik=0 ; ik<partIn.size() ; ++ik )
    {
        prePush1D( partIn[ik], partOut[ik], dto2dx ) ;
    }

    for( uint32 ik=0 ; ik<partOut.size() ; ++ik )
    {
        compute1DFieldsAtParticles( interpolator, partOut[ik],
                                    layout_, E, B ) ;
    }

    for( uint32 ik=0 ; ik<partOut.size() ; ++ik )
    {
        pushVelocity1D( partOut[ik], partOut[ik], dt, m );
    }


    for( uint32 ik=0 ; ik<partOut.size() ; ++ik )
    {
        corPush1D( partOut[ik], partOut[ik], dto2dx );
    }

}



void ModifiedBoris::move2D( std::vector<Particle> & partIn ,
                            std::vector<Particle> & partOut,
                            double dt, double m,
                            VecField const & E ,
                            VecField const & B ,
                            Interpolator & interpolator )
{

}



void ModifiedBoris::move3D( std::vector<Particle> & partIn ,
                            std::vector<Particle> & partOut,
                            double dt, double m,
                            VecField const & E ,
                            VecField const & B ,
                            Interpolator & interpolator )
{

}



void ModifiedBoris::prePush1D( Particle & part_tn,
                               Particle & part_tpred,
                               double dto2dx )
{
    // time decentering of the delta position at tn+1/2
    float delta = part_tn.delta[0] + static_cast<float>( dto2dx * part_tn.v[0] ) ;

    // check the validity of delta (0 <= delta <= 1)
    // and do auto-correction
    float iPart = std::floor(delta) ;
    part_tpred.delta[0] = delta - iPart ;

    // update the logical node
    part_tpred.icell[0] += iPart ;
}



void ModifiedBoris::pushVelocity1D( Particle & part_tn,
                                    Particle & part_tp,
                                    double dt, double m )
{
    double dto2 = 0.5*dt ;

    double coef1 = part_tn.charge * dto2/m ;

    // We now apply the 3 steps of the BORIS PUSHER

    // 1st half push of the electric field
    double velx1 = part_tn.v[0] + coef1*part_tn.Ex ;
    double vely1 = part_tn.v[1] + coef1*part_tn.Ey ;
    double velz1 = part_tn.v[2] + coef1*part_tn.Ez ;


    // preparing variables for magnetic rotation
    double rx = coef1 *part_tn.Bx ;
    double ry = coef1 *part_tn.By ;
    double rz = coef1 *part_tn.Bz ;

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
    velx1 = velx2 + coef1*part_tn.Ex ;
    vely1 = vely2 + coef1*part_tn.Ey ;
    velz1 = velz2 + coef1*part_tn.Ez ;

    // Update particle velocity
    part_tp.v[0] = velx1 ;
    part_tp.v[1] = vely1 ;
    part_tp.v[2] = velz1 ;

}


void ModifiedBoris::corPush1D( Particle & part_tpred,
                               Particle & part_tcor,
                               double dto2dx )
{
    // we update the delta position at tn+1
    float delta = part_tpred.delta[0] + static_cast<float>( dto2dx * part_tpred.v[0] ) ;

    // check the validity of delta (0 <= delta <= 1)
    // and do auto-correction
    float iPart = std::floor(delta) ;
    part_tcor.delta[0] = delta - iPart ;

    // update the logical node
    part_tcor.icell[0] += iPart ;
}




