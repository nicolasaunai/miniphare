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


void Species::computeChargeDensityAndFlux( Projector & project   )
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
        project.getIndexesAndWeights( part, Direction::X ) ;

        std::vector<uint32> ISx    = std::get<0>(indexesAndWeights) ;
        std::vector<double> PondSx = std::get<1>(indexesAndWeights) ;

        for( uint32 ik=0 ; ik<ISx.size() ; ++ik )
        {
            rho_( ISx[ik] ) += aux_rh * PondSx[ik] ;

            vx( ISx[ik] ) += aux_vx * PondSx[ik] ;
            vy( ISx[ik] ) += aux_vy * PondSx[ik] ;
            vz( ISx[ik] ) += aux_vz * PondSx[ik] ;
        }
    }

}



void Species::computeFieldsAtParticles( Interpolator & interp,
                                        VecField const & E ,
                                        VecField const & B )
{
    uint32 idirX = static_cast<uint32>(Direction::X) ;
    uint32 idirY = static_cast<uint32>(Direction::Y) ;
    uint32 idirZ = static_cast<uint32>(Direction::Z) ;

    Field const & Ex = E.component(idirX) ;
    Field const & Ey = E.component(idirY) ;
    Field const & Ez = E.component(idirZ) ;

    Field const & Bx = B.component(idirX) ;
    Field const & By = B.component(idirY) ;
    Field const & Bz = B.component(idirZ) ;

    for( Particle & part : particleArray_)
    {
        auto indexesAndWeightsPrimal = \
        interp.getIndexesAndWeights( part, Direction::X, QtyCentering::primal ) ;

        auto indexesAndWeightsDual = \
        interp.getIndexesAndWeights( part, Direction::X, QtyCentering::dual ) ;

        std::vector<uint32> ISx_p    = std::get<0>(indexesAndWeightsPrimal) ;
        std::vector<double> PondSx_p = std::get<1>(indexesAndWeightsPrimal) ;

        std::vector<uint32> ISx_d    = std::get<0>(indexesAndWeightsDual) ;
        std::vector<double> PondSx_d = std::get<1>(indexesAndWeightsDual) ;

        part.Ex = 0. ; part.Ey = 0. ; part.Ez = 0. ;
        part.Bx = 0. ; part.By = 0. ; part.Bz = 0. ;

        auto centering = layout_.fieldCentering( Ex, Direction::X ) ;

        auto indexesAndWeights = \
        interp.getIndexesAndWeights( part, Direction::X, centering ) ;

        std::vector<uint32> ISx    = std::get<0>(indexesAndWeights) ;
        std::vector<double> PondSx = std::get<1>(indexesAndWeights) ;



    }



}

