
#include <cmath>
#include <memory>

#include "Field/field.h"
#include "Plasmas/ions.h"
#include "Solver/solver.h"
#include "grid/gridlayout.h"
#include "Plasmas/electrons.h"
#include "pusher/pusherfactory.h"
#include "Faraday/faradayfactory.h"
#include "Interpolator/interpolator.h"

#include "vecfield/vecfieldoperations.h"



Solver::Solver( GridLayout const& layout, double dt,
                std::unique_ptr<SolverInitializer> solverInitializer )
    :layout_{layout},
      EMFieldsPred_{{ {layout.allocSize(HybridQuantity::Ex ),
                    layout.allocSize(HybridQuantity::Ey ),
                    layout.allocSize(HybridQuantity::Ez )  }},

                    { {layout.allocSize(HybridQuantity::Bx ),
                    layout.allocSize(HybridQuantity::By ),
                    layout.allocSize(HybridQuantity::Bz )  }}, "_pred"},

      EMFieldsAvg_{{ {layout.allocSize(HybridQuantity::Ex ),
                   layout.allocSize(HybridQuantity::Ey ),
                   layout.allocSize(HybridQuantity::Ez )  }},

                   { {layout.allocSize(HybridQuantity::Bx ),
                   layout.allocSize(HybridQuantity::By ),
                   layout.allocSize(HybridQuantity::Bz )  }}, "_avg" },

      Jtot_{ layout.allocSize(HybridQuantity::Ex),
             layout.allocSize(HybridQuantity::Ey),
             layout.allocSize(HybridQuantity::Ez),
             { {HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez} },
             "Jtot" },
      faraday_{dt, layout},
      ampere_{layout},
      ohm_{layout}
{

    uint32 size = static_cast<uint32> ( solverInitializer->interpolationOrders.size() ) ;
    for( uint32 ik=0 ; ik<size ; ++ik )
    {
        uint32 order = solverInitializer->interpolationOrders[ik] ;
        interpolators_.push_back( std::unique_ptr<Interpolator>(new Interpolator(order)) ) ;
    }

    const std::string pusherType = solverInitializer->pusherType ;
    pusher_ =  PusherFactory::createPusher( layout, pusherType, dt) ;

    // TODO need to initialize OHM object
    // TODO and vector (?) of particles (n+1)
}






void Solver::solveStep(Electromag& EMFields, Ions& ions,
                       Electrons& electrons,
                       BoundaryCondition const * const boundaryCondition )
{
    VecField &B      = EMFields.getB();
    VecField &E      = EMFields.getE();
    VecField &Bpred  = EMFieldsPred_.getB();
    VecField &Epred  = EMFieldsPred_.getE();
    VecField &Bavg   = EMFieldsAvg_.getB();
    VecField &Eavg   = EMFieldsAvg_.getE();



    // -----------------------------------------------------------------------
    //
    //                              PREDICTOR 1
    //
    // -----------------------------------------------------------------------

    // Get B^{n+1} pred1 from E^n
    faraday_(E, B, Bpred);
    boundaryCondition->applyMagneticBC( Bpred ) ;

    // Compute J
    ampere_(Bpred, Jtot_);
    boundaryCondition->applyCurrentBC(Jtot_);


    // --> Get electron moments at time n
    VecField const& Vepred1 = electrons.bulkVel(ions.bulkVel(), ions.rho(), Jtot_);
    Field const&    PePred1 = electrons.pressure( ions.rho() );

    // --> Get electric field E_{n+1} pred1 from Ohm's law
    ohm_(Bpred, ions.rho(), Vepred1, PePred1, Jtot_, Epred);
    boundaryCondition->applyElectricBC( Epred ) ;


    // Get time averaged prediction (E,B)^{n+1/2} pred1
    // using (E^n, B^n) and (E^{n+1}, B^{n+1}) pred1
    average(E, Epred, Eavg, layout_ );
    average(B, Bpred, Bavg, layout_ );


    // Move ions from n to n+1 using (E^{n+1/2},B^{n+1/2}) pred 1
    // accumulate moments for each species and total ions.
    // last argument is TRUE so that particles at n+1 are stored
    // in a temporary buffer and particles at n are kept at n
    moveIons_(Eavg, Bavg, ions, boundaryCondition, true);


    // -----------------------------------------------------------------------
    //
    //                         PREDICTOR 2
    //
    // -----------------------------------------------------------------------


    // Get B^{n+1} pred2 from E^{n+1/2} pred1
    faraday_(Eavg, B, Bpred);
    boundaryCondition->applyMagneticBC( Bpred ) ;

    // Compute J
    ampere_(Bpred, Jtot_) ;
    boundaryCondition->applyCurrentBC( Jtot_ ) ;


    // --> Get electron moments at time n with Pred1 ion moments
    VecField const& Vepred2 = electrons.bulkVel(ions.bulkVel(), ions.rho(), Jtot_);
    Field const&    PePred2 = electrons.pressure( ions.rho() );

    // --> Get electric field E^{n+1} pred2 from Ohm's law
    // --> using (n^{n+1}, u^{n+1}) pred and B_{n+1} pred2
    ohm_(Bpred, ions.rho(), Vepred2, PePred2, Jtot_, Epred);
    boundaryCondition->applyElectricBC( Epred ) ;


    // --> Get time averaged prediction (E^(n+1/2),B^(n+1/2)) pred2
    // --> using (E^n, B^n) and (E^{n+1}, B^{n+1}) pred2
    average( E, Epred, Eavg, layout_ );
    average( B, Bpred, Bavg, layout_ );


    // Get the CORRECTED positions and velocities
    // Move ions from n to n+1 using (E^{n+1/2},B^{n+1/2}) pred2
    // Last argument here is FALSE because we want to
    // update ions at n+1 in place, i.e. overwritting ions at n
    moveIons_(Eavg, Bavg, ions, boundaryCondition, false);


    // -----------------------------------------------------------------------
    //
    //                           CORRECTOR
    //
    // -----------------------------------------------------------------------


    // Get CORRECTED B^{n+1} from E^{n+1/2} pred2
    faraday_(Eavg, B, B);
    boundaryCondition->applyMagneticBC(B);

    // Compute J
    ampere_(B, Jtot_) ;
    boundaryCondition->applyCurrentBC(Jtot_) ;


    // --> Get electron moments at time n with Pred2 ion moments
    VecField const& Vecorr = electrons.bulkVel(ions.bulkVel(), ions.rho(), Jtot_);
    Field const&    Pecorr = electrons.pressure( ions.rho() );

    // --> Get CORRECTED electric field E^{n+1} from Ohm's law
    // --> using (n^{n+1}, u^{n+1}) cor and B_{n+1} cor
    ohm_(B, ions.rho(), Vecorr, Pecorr, Jtot_, E);
    // BC Fields --> Apply boundary conditions on the electric field
    boundaryCondition->applyElectricBC( E ) ;

}



// convenience function that counts the maximum number of particles over
// all species. this is useful to allocated the temporary particle buffer
std::vector<Particle>::size_type maxNbrParticles(Ions const& ions)
{
    // find the largest particles number accross all species
    auto nbrParticlesMax = ions.species(0).nbrParticles();
    auto nbrSpecies = ions.nbrSpecies();

    for (uint32 ispe=0; ispe < nbrSpecies; ++ispe)
    {
        Species const& species = ions.species(ispe);
        auto nbrParticles = species.nbrParticles();

        if (nbrParticlesMax < nbrParticles)
        {
            nbrParticlesMax = nbrParticles;
        }
    }
    return nbrParticlesMax;
}




// this routine move the ions for all species, accumulate their moments
// and compute the total ion moments.
void Solver::moveIons_(VecField const& E, VecField const& B, Ions& ions,
                       BoundaryCondition const * const boundaryCondition,
                       bool pred1)
{


    // the temporary buffer must be big enough to hold the max
    // number of particles
    auto nbrParticlesMax = maxNbrParticles(ions);
    if (particleArrayPred_.size() < nbrParticlesMax)
    {
        particleArrayPred_.resize(nbrParticlesMax);
    }


    for (uint32 ispe=0; ispe < ions.nbrSpecies(); ++ispe)
    {
        Species& species                 = ions.species(ispe);
        std::vector<Particle>& particles = species.particles();
        Interpolator& interpolator       = *interpolators_[ispe];


        // at the first predictor step we must not overwrite particles
        // at t=n with particles at t=n+1 because time n will be used
        // in the second push. we rather put particles at time n+1 in a
        // temporary buffer particleArrayPred_
        if (pred1)
        {
            // move all particles of that species from n to n+1
            // and put the advanced particles in the predictor buffer 'particleArrayPred_'
            pusher_->move(particles, particleArrayPred_, species.mass(), E, B, interpolator);

            // resize the buffer so that charge density and fluxes use
            // no more than the right number of particles
            // particleArrayPred_ has a capacity that is large enough for all
            // particle arrays for all species.
            particleArrayPred_.resize(particles.size());
            boundaryCondition->applyParticleBC(particleArrayPred_,
                                               pusher_->getLeavingParticles());

            computeChargeDensityAndFlux(interpolator, species, layout_, particleArrayPred_);
        }

        // we're at pred2, so we can update particles in place as we won't
        // need their properties at t=n anymore
        else
        {
            // move all particles of that species from n to n+1
            pusher_->move(particles, particles, species.mass(), E, B, interpolator);
            boundaryCondition->applyParticleBC(particles,
                                               pusher_->getLeavingParticles());
            computeChargeDensityAndFlux(interpolator, species, layout_, particles);
        }



    } // end loop on species


    ions.computeChargeDensity();
    ions.computeBulkVelocity();

    boundaryCondition->applyDensityBC(ions.rho());
    boundaryCondition->applyBulkBC(ions.bulkVel());
}





