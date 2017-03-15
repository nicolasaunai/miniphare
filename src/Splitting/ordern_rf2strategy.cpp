

#include "Splitting/ordern_rf2strategy.h"

#include "Distributions/distribgenerator.h"
#include "Distributions/distribstrategy.h"
#include "Distributions/uniformstrategy.h"


OrderN_RF2Strategy::OrderN_RF2Strategy( const std::string & splitMethod )
    : SplittingStrategy(splitMethod) {}


std::vector<Particle> OrderN_RF2Strategy::split(
        double dxL1, uint32 refineFactor,
        uint32 interpOrder,
        const std::vector<Particle> & motherParticles ) const
{

    std::vector<Particle> newParticles ;

    uint64 nbpart_L1 = 0 ;

    int32 nbpts = (2*refineFactor -1) + (refineFactor -1)*(interpOrder-1) ;

    std::vector<double> w_tab    (static_cast<uint32>(nbpts), 0.) ;
    std::vector<double> delta_tab(static_cast<uint32>(nbpts), 0.) ;

    for( int ik=0 ; ik<nbpts ; ik++ )
    {
        w_tab[ik] = static_cast<double>( binomial(interpOrder+1, ik) )/
                static_cast<double>( ipow(refineFactor, interpOrder) ) ;
    }

    double wtot = 0. ;
    for( double weight : w_tab )
    {
        wtot += weight ;
    }

    double ix = -(nbpts - 1)/2. ;
    for( double & delta : delta_tab )
    {
        delta = ix * dxL1 ;
        ix = ix + 1. ;
    }


    for( const Particle & part : motherParticles )
    {
        double  mum_weight = part.getP_po() ;
        double  mum_posx   = part.getP_qx() ;
        std::array<double, 3> mum_vel = { {part.getP_vx(), part.getP_vy(), part.getP_vz()} };

        std::vector<double> posx_tab(static_cast<uint32>(nbpts), 0.) ;
        std::vector<double> wn_tab  (static_cast<uint32>(nbpts), 0.) ;

        for( int ik=0 ; ik<nbpts ; ik++ )
        {
            posx_tab[ik] = mum_posx + delta_tab[ik] ;

            wn_tab[ik] = mum_weight * w_tab[ik]/wtot ;
        }

        for( int ik=0 ; ik<nbpts ; ik++ )
        {
            Particle partBaby( wn_tab[ik], posx_tab[ik], mum_vel) ;

            newParticles.push_back( partBaby );
        }

        nbpart_L1 += static_cast<uint64>(nbpts) ;
    }

    std::cout << "Nombre de particules L0 = " << motherParticles.size() << "\n" << std::endl ;
    std::cout << "Nombre de particules L1 = " << nbpart_L1 << "\n" << std::endl ;

    return  newParticles ;
}




