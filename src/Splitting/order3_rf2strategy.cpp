#include "order3_rf2strategy.h"

#include "Distributions/distribgenerator.h"
#include "Distributions/distribstrategy.h"
#include "Distributions/uniformstrategy.h"


Order3Strategy::Order3Strategy( const std::string & splitMethod )
    : SplittingStrategy(splitMethod) {}


std::vector<Particle> Order3Strategy::split(
        const GlobalParams & globalParams  ,
        uint64 totalNbrParticles_          ,
        double dxL1,
        const std::vector<Particle> & motherParticles ) const
{

    double xmin = globalParams.minGlobX() ;
    double xmax = globalParams.maxGlobX() ;

    uint32 refineFactor = globalParams.refineFactor() ;

    std::vector<Particle> newParticles ;

    uint64 nbpart_L1 = 0 ;


    double w1 = 1./8. ;
    double w2 = 4./8. ;
    double w3 = 6./8. ;
    double w4 = 4./8. ;
    double w5 = 1./8. ;

    double wtot = w1 + w2 + w3 + w4 + w5 ;


    for( const Particle & part : motherParticles )
    {
        double  mum_weight = part.getP_po() ;
        double  mum_posx   = part.getP_qx() ;
        std::array<double, 3> mum_vel = { {part.getP_vx(), part.getP_vy(), part.getP_vz()} } ;

        switch( refineFactor )
        {
        case 2:
        {
            double posx1 = mum_posx - 2.*dxL1 ;
            double posx2 = mum_posx -    dxL1 ;
            double posx3 = mum_posx            ;
            double posx4 = mum_posx +    dxL1 ;
            double posx5 = mum_posx + 2.*dxL1 ;

            double wn1 = mum_weight * w1/wtot ;
            double wn2 = mum_weight * w2/wtot ;
            double wn3 = mum_weight * w3/wtot ;
            double wn4 = mum_weight * w4/wtot ;
            double wn5 = mum_weight * w5/wtot ;

            bool valid_pos = false ;
            // Only check extreme points
            if( posx1>xmin && posx1<xmax &&
                posx5>xmin && posx5<xmax )
            {
                valid_pos = true ;
            }

            if( valid_pos )
            {
                Particle partBaby1( wn1, posx1, mum_vel) ;
                Particle partBaby2( wn2, posx2, mum_vel) ;
                Particle partBaby3( wn3, posx3, mum_vel) ;
                Particle partBaby4( wn4, posx4, mum_vel) ;
                Particle partBaby5( wn5, posx5, mum_vel) ;

                newParticles.push_back( partBaby1 );
                newParticles.push_back( partBaby2 );
                newParticles.push_back( partBaby3 );
                newParticles.push_back( partBaby4 );
                newParticles.push_back( partBaby5 );

                nbpart_L1 += 5 ;
            }
        }
            break;

        default:
            std::cout << "Method  Species::splitOrdre2 : default case !" << std::endl ;
            std::cout << "FATAL ERROR !" << std::endl ;
            std::exit(0) ;
            break;
        }


    }

    std::cout << "Nombre de particules L0 = " << totalNbrParticles_ << "\n" << std::endl ;
    std::cout << "Nombre de particules L1 = " << nbpart_L1 << "\n" << std::endl ;

    return newParticles ;
}


