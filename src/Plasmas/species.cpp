#include "grid/gridlayoutdefs.h"

#include "species.h"



Species::Species(GridLayout const& layout, double mass,
                 std::unique_ptr<ParticleInitializer> particleInitializer,
                 std::string const& name)
    : mass_{mass},
      name_{name},
      layout_{ layout },
      rho_     { layout.allocSize(HybridQuantity::rho), HybridQuantity::rho, "_rhoTot" },
      flux_    { layout.allocSize(HybridQuantity::V),
                 layout.allocSize(HybridQuantity::V),
                 layout.allocSize(HybridQuantity::V),
                 { {HybridQuantity::V, HybridQuantity::V, HybridQuantity::V} },
                 "_fluxSpecies" },
      particleArray_{},
      particleInitializer_{ std::move(particleInitializer) } //TODO broken copy
{
    // TODO should check stuff here.
    // like : is particleInitializer OK?
}



void Species::loadParticles()
{
    particleInitializer_->loadParticles(particleArray_);
}


void Species::computeChargeDensityAndCurrents( Projector & project   )
{

    uint32 idirX = static_cast<uint32>(Direction::X) ;
    uint32 idirY = static_cast<uint32>(Direction::Y) ;
    uint32 idirZ = static_cast<uint32>(Direction::Z) ;

    Field & jx = (*this).flux(idirX) ;
    Field & jy = (*this).flux(idirY) ;
    Field & jz = (*this).flux(idirZ) ;


    this->resetMoments() ;

    for( const Particle & part : particleArray_)
    {

        double aux_rh = part.weight * part.charge * layout_.odx() ;

        double aux_cx = aux_rh * part.v[0] ;
        double aux_cy = aux_rh * part.v[1] ;
        double aux_cz = aux_rh * part.v[2] ;

        auto indexesAndWeights = \
        project.getIndexesAndWeights( part, Direction::X ) ;

        std::vector<uint32> ISx    = std::get<0>(indexesAndWeights) ;
        std::vector<double> PondSx = std::get<1>(indexesAndWeights) ;

        for( uint32 ik=0 ; ik<ISx.size() ; ++ik )
        {
            rho_( ISx[ik] ) += aux_rh * PondSx[ik] ;

            jx( ISx[ik] ) += aux_cx * PondSx[ik] ;
            jy( ISx[ik] ) += aux_cy * PondSx[ik] ;
            jz( ISx[ik] ) += aux_cz * PondSx[ik] ;
        }
    }

}


