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


void Species::compute1DChargeDensityAndFlux( Interpolator & project   )
{
    uint32 idirX = static_cast<uint32>(Direction::X) ;
    uint32 idirY = static_cast<uint32>(Direction::Y) ;
    uint32 idirZ = static_cast<uint32>(Direction::Z) ;

    Field & vx = (*this).flux(idirX) ;
    Field & vy = (*this).flux(idirY) ;
    Field & vz = (*this).flux(idirZ) ;


    this->resetMoments() ;

    for( const Particle & part : particleArray_)
    {

        double aux_rh   = part.weight * part.charge * layout_.odx() ;
        double aux_flux = part.weight * layout_.odx() ;

        double aux_vx = aux_flux * part.v[0] ;
        double aux_vy = aux_flux * part.v[1] ;
        double aux_vz = aux_flux * part.v[2] ;

        auto indexesAndWeights = \
        project.getIndexesAndWeights(part, Direction::X ) ;

        std::vector<uint32> indexes    = std::get<0>(indexesAndWeights) ;
        std::vector<double> weights = std::get<1>(indexesAndWeights) ;

        for( uint32 ik=0 ; ik<indexes.size() ; ++ik )
        {
            rho_( indexes[ik] ) += aux_rh * weights[ik] ;

            vx( indexes[ik] ) += aux_vx * weights[ik] ;
            vy( indexes[ik] ) += aux_vy * weights[ik] ;
            vz( indexes[ik] ) += aux_vz * weights[ik] ;
        }
    }

}




