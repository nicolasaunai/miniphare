

#include "Splitting/order1_rfnstrategy.h"

#include "Distributions/distribgenerator.h"
#include "Distributions/distribstrategy.h"
#include "Distributions/uniformstrategy.h"


Order1_RFnStrategy::Order1_RFnStrategy( const std::string & splitMethod )
    : SplittingStrategy(splitMethod) {}


std::vector<Particle> Order1_RFnStrategy::split1D(
        double dxL1, uint32 refineFactor,
        const std::vector<Particle> & motherParticles ) const
{

    std::vector<Particle> newParticles ;

    uint64 nbpart_L1 = 0 ;


    uint32 nbpts = (2*refineFactor -1) ;

    std::vector<double> w_tab    (nbpts, 0.) ;
    std::vector<double> delta_tab(nbpts, 0.) ;

    int inum=1 ;
    for( unsigned int ik=0 ; ik<nbpts ; ik++ )
    {
        w_tab[ik] = static_cast<double>(inum)/static_cast<double>(refineFactor) ;

        if( static_cast<int>(ik-refineFactor+1) < 0 )
            inum++ ;
        else
            inum-- ;
    }

    double wtot = 0. ;
    for( double weight : w_tab )
    {
        wtot += weight ;
    }

    int ix = -(static_cast<int>(refineFactor) - 1) ;
    for( double & delta : delta_tab )
    {
        delta = ix * dxL1 ;
        ix++ ;
    }


    for( const Particle & part : motherParticles )
    {
        double  mum_weight = part.getP_po() ;
        double  mum_posx   = part.getP_qx() ;
        std::array<double, 3> mum_vel = { {part.getP_vx(), part.getP_vy(), part.getP_vz()} };

        std::vector<double> posx_tab(nbpts, 0.) ;
        std::vector<double> wn_tab  (nbpts, 0.) ;

        for( unsigned int ik=0 ; ik<nbpts ; ik++ )
        {
            posx_tab[ik] = mum_posx + delta_tab[ik] ;

            wn_tab[ik] = mum_weight * w_tab[ik]/wtot ;
        }


        for( unsigned int ik=0 ; ik<nbpts ; ik++ )
        {
            Particle partBaby( wn_tab[ik], posx_tab[ik], mum_vel) ;

            newParticles.push_back( partBaby );
        }

        nbpart_L1 += nbpts ;
    }

    std::cout << "Nombre de particules L0 = " << motherParticles.size() << "\n" << std::endl ;
    std::cout << "Nombre de particules L1 = " << nbpart_L1 << "\n" << std::endl ;


    return newParticles ;
}


