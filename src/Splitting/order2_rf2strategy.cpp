

#include "Splitting/order2_rf2strategy.h"

#include "Distributions/distribgenerator.h"
#include "Distributions/distribstrategy.h"
#include "Distributions/uniformstrategy.h"


Order2_RF2Strategy::Order2_RF2Strategy( const std::string & splitMethod )
    : SplittingStrategy(splitMethod) {}


std::vector<Particle> Order2_RF2Strategy::split(
        double dxL1, uint32 refineFactor,
        const std::vector<Particle> & motherParticles ) const
{

    std::vector<Particle> newParticles ;

    uint64 nbpart_L1 = 0 ;


    double w1 = 1./4. ;
    double w2 = 3./4. ;
    double w3 = 3./4. ;
    double w4 = 1./4. ;

    double wtot = w1 + w2 + w3 + w4 ;

    for( const Particle & part : motherParticles )
    {
        double  mum_weight = part.getP_po() ;
        double  mum_posx   = part.getP_qx() ;
        std::array<double, 3> mum_vel = { {part.getP_vx(), part.getP_vy(), part.getP_vz()} } ;

        switch( refineFactor )
        {
        case 2:
        {
            double posx1 = mum_posx - 1.5*dxL1 ;
            double posx2 = mum_posx - 0.5*dxL1 ;
            double posx3 = mum_posx + 0.5*dxL1 ;
            double posx4 = mum_posx + 1.5*dxL1 ;

            double wn1 = mum_weight * w1/wtot ;
            double wn2 = mum_weight * w2/wtot ;
            double wn3 = mum_weight * w3/wtot ;
            double wn4 = mum_weight * w4/wtot ;

            Particle partBaby1( wn1, posx1, mum_vel) ;
            Particle partBaby2( wn2, posx2, mum_vel) ;
            Particle partBaby3( wn3, posx3, mum_vel) ;
            Particle partBaby4( wn4, posx4, mum_vel) ;

            newParticles.push_back( partBaby1 );
            newParticles.push_back( partBaby2 );
            newParticles.push_back( partBaby3 );
            newParticles.push_back( partBaby4 );

            nbpart_L1 += 4 ;
        }
            break;

        default:
            std::cout << "Method  Species::splitOrdre2 : default case !" << std::endl ;
            std::cout << "FATAL ERROR !" << std::endl ;
            std::exit(0) ;
            break;
        }


    }

    std::cout << "Nombre de particules L0 = " << motherParticles.size() << "\n" << std::endl ;
    std::cout << "Nombre de particules L1 = " << nbpart_L1 << "\n" << std::endl ;

    return newParticles ;
}


