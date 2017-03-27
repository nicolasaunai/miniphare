

#include "Splitting/approx_1to4strategy.h"

#include "Distributions/distribgenerator.h"
#include "Distributions/distribstrategy.h"
#include "Distributions/uniformstrategy.h"


Approx_1to4Strategy::Approx_1to4Strategy(const std::string & splitMethod,
                                         double ratioDx)
    : SplittingStrategy(splitMethod), ratioDx_{ratioDx} {}


std::vector<Particle> Approx_1to4Strategy::split1D(
        double dxL1, uint32 refineFactor,
        const std::vector<Particle> & motherParticles ) const
{

    std::vector<double> jitterX ;
    std::vector<Particle> newParticles ;

    uint64 nbpart_L1 = 0 ;

    const double nb_children = 4. ;



    // we prepare random jitter for child particles
    auto distGenerator = std::make_shared< DistribGenerator >();
    std::shared_ptr<DistribStrategy> uniform ( std::make_shared<UniformStrategy>() );

    distGenerator->setStrategy(uniform);
    // We draw a uniform on the segment [0., 1.]
    distGenerator->draw(jitterX, motherParticles.size(), 0., 1.);

    // We limit jitter to 10% of the local(refined) cell size
    for( double & jitx : jitterX )
    {
        jitx = jitx * dxL1 * ratioDx_ ;
    }

    unsigned int ik_mum = 0 ;
    for( const Particle & part : motherParticles )
    {
        double  mum_weight = part.getP_po() ;
        double  mum_posx   = part.getP_qx() ;
        std::array<double, 3> mum_vel = { {part.getP_vx(), part.getP_vy(), part.getP_vz()} } ;

        switch( refineFactor )
        {
        case 2:
        {
            double weight = mum_weight/nb_children ;

            // creation of particles 1 and 2
            double posx1 = mum_posx + 0.5*jitterX[ik_mum] ;
            double posx2 = mum_posx - 0.5*jitterX[ik_mum] ;

            Particle partBaby1( weight, posx1, mum_vel) ;
            Particle partBaby2( weight, posx2, mum_vel) ;
            newParticles.push_back( partBaby1 );
            newParticles.push_back( partBaby2 );
            nbpart_L1 += 2 ;

            // creation of particles 3 and 4
            posx1 = mum_posx + 1.5*jitterX[ik_mum] ;
            posx2 = mum_posx - 1.5*jitterX[ik_mum] ;

            Particle partBaby1( weight, posx1, mum_vel) ;
            Particle partBaby2( weight, posx2, mum_vel) ;
            newParticles.push_back( partBaby1 );
            newParticles.push_back( partBaby2 );
            nbpart_L1 += 2 ;
        }
            break;

        default:
            std::cout << "Split particles : default case !" << std::endl ;
            break;
        }

        ik_mum++ ;
    }


    std::cout << "Nombre de particules L0 = " << totalNbrParticles_ << "\n" << std::endl ;
    std::cout << "Nombre de particules L1 = " << nbpart_L1 << "\n" << std::endl ;

    return newParticles ;
}





