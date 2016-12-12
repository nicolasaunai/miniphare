
#include <cmath>

#include "pusher/modifiedboris.h"

#include "Interpolator/interpolator.h"

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
                            VecField const & E , VecField const & B)
{
    partOut = partIn ;

    for( uint32 ik=0 ; ik<partIn.size() ; ++ik )
    {
        prePush1D( partIn[ik], partOut[ik], dt ) ;
    }

    // TODO take the solver interpolator rather than this local one
    std::unique_ptr<Interpolator> \
            interpol{ new Interpolator( layout_)} ;

    for( uint32 ik=0 ; ik<partOut.size() ; ++ik )
    {
        compute1DFieldsAtParticles( *interpol, partOut[ik],
                                    layout_, E, B ) ;
    }

    for( uint32 ik=0 ; ik<partOut.size() ; ++ik )
    {
        pushVelocity1D( partOut[ik], partOut[ik], dt, m );
    }


    for( uint32 ik=0 ; ik<partOut.size() ; ++ik )
    {
        corPush1D( partOut[ik], partOut[ik], dt );
    }

}



void ModifiedBoris::move2D( std::vector<Particle> & partIn ,
                            std::vector<Particle> & partOut,
                            double dt, double m,
                            VecField const & E ,
                            VecField const & B )
{

}



void ModifiedBoris::move3D( std::vector<Particle> & partIn ,
                            std::vector<Particle> & partOut,
                            double dt, double m,
                            VecField const & E ,
                            VecField const & B )
{

}



void ModifiedBoris::prePush1D( Particle & part_tn,
                               Particle & part_tp,
                               double dt )
{
    double dto2 = 0.5*dt ;

    // position at time tn
    double posx = ( part_tn.icell[0] + static_cast<double>(part_tn.delta[0]) )*dx_ ;

    // time decentering position at tn+1/2
    double x_pred = posx + dto2* part_tn.v[0] ;

    // get the node coordinate and the delta
    double integerPart = 0. ;
    part_tp.delta[0] = static_cast<float>( std::modf(x_pred/dx_, &integerPart) ) ;
    part_tp.icell[0] = static_cast<uint32>( integerPart ) ;

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



void ModifiedBoris::corPush1D( Particle & part_tp,
                               Particle & part_tcor,
                               double dt )
{
    double dto2 = 0.5*dt ;

    // position at time tpred
    double x_pred = ( part_tp.icell[0] + static_cast<double>(part_tp.delta[0]) )*dx_ ;

    // we update the position at tn+1
    double posx = x_pred + dto2 * part_tp.v[0] ;

    // TODO later handle the origin of a patch
    //    particle.position[0] = posx ;

    // get the node coordinate and the delta
    double integerPart = 0. ;
    part_tcor.delta[0] = static_cast<float>( std::modf(posx/dx_, &integerPart) ) ;
    part_tcor.icell[0] = static_cast<uint32>( integerPart ) ;

}




