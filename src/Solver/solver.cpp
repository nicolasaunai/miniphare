
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
      ampere_{layout}
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



    // --> Get B_{n+1} pred1 from E^n
    faraday_(E, B, Bpred);
    // BC Fields --> Apply boundary conditions on the electric field
    boundaryCondition->applyMagneticBC( Bpred ) ;

    // Compute J
    ampere_(Bpred, Jtot_) ;
    // BC on the current
   // boundaryConditions_.applyCurrentBC( Jtot_ ) ;

    // --> MOMENTS (n^n, u^n) at time n have
    // --> already been computed, or are known just after initialization
    // --> Get ion and electron moments at time n

    // --> Get electric field E_{n+1} pred1 from Ohm's law
    // --> using (n^n, u^n) and B_{n+1}
    // ohm(Bpred, Ne, Ve, Pe, Epred);

    // BC Fields --> Apply boundary conditions on the electric field
    //boundaryConditions_.applyElectricBC( Epred ) ;

    // --> Get time averaged prediction (E,B)_(n+1/2) pred1
    // --> using (E^n, B^n) and (E^{n+1}, B^{n+1}) pred1

    average(E, Epred, Eavg, layout_ );
    average(B, Bpred, Bavg, layout_ );

    // --> Move ions from n to n+1 using (E^{n+1/2},B^{n+1/2}) pred 1
    moveIons_(Eavg, Bavg, ions, boundaryCondition);

    // BC Parts --> Apply boundary conditions on particles

    // --> Get B^{n+1} pred2 from E^{n+1/2} pred1
    faraday_(Eavg, B, Bpred);
    // BC Fields --> Apply boundary conditions
    //boundaryConditions_.applyMagneticBC( Bpred ) ;

    // Compute J
    ampere_(Bpred, Jtot_) ;
    // BC on the current
    //boundaryConditions_.applyCurrentBC( Jtot_ ) ;

    // --> DEPOSIT PREDICTED MOMENTS (n^{n+1}, u^{n+1}) AT TIME n+1
    // --> get ion and electron moments at time n+1 (pred 1)
    // Field const& Ni = ions.chargeDensity();
    // VecField const& Vi = ions.bulkVelocity();

    // VecField const& Ve = electrons.bulkVelocity(B, Vi, Ni);
    // Field const& Ne  = electrons.chargeDensity();
    // --> Calculate the electron pressure tensor
    // --> from the electron closure
    // Field const& Pe = electrons.Pressure(/* Ni ? */ );

    // --> Get electric field E^{n+1} pred2 from Ohm's law
    // --> using (n^{n+1}, u^{n+1}) pred and B_{n+1} pred2
    // ohm(Bpred, Ne, Ve, Pe, Epred);

    // BC Fields --> Apply boundary conditions on the electric field
   // boundaryConditions_.applyElectricBC( Epred ) ;

    // --> Get time averaged prediction (E^(n+1/2),B^(n+1/2)) pred2
    // --> using (E^n, B^n) and (E^{n+1}, B^{n+1}) pred2
    average( E, Epred, Eavg, layout_ );
    average( B, Bpred, Bavg, layout_ );

    // --> Get the CORRECTED positions and velocities
    // --> Move ions from n to n+1 using (E^{n+1/2},B^{n+1/2}) pred2
    moveIons_(Eavg, Bavg, ions, boundaryCondition);

    // BC Parts --> Apply boundary conditions on particles

    // --> Get CORRECTED B^{n+1} from E^{n+1/2} pred2
    faraday_(Eavg, B, B);
    // BC Fields --> Apply boundary conditions
    //boundaryConditions_.applyMagneticBC( B ) ;

    // Compute J
    ampere_(B, Jtot_) ;
    // BC on the current
    //boundaryConditions_.applyCurrentBC( Jtot_ ) ;

    // --> DEPOSIT CORRECTED MOMENTS (n^{n+1}, u^{n+1})
    // --> Get ion and electron moments at time n+1
    // Field const& Ni = ions.chargeDensity();
    // VecField const& Vi = ions.bulkVelocity();

    // VecField const& Ve = electrons.bulkVelocity(B, Vi, Ni);
    // Field const& Ne  = electrons.chargeDensity();
    // --> Calculate the electron pressure tensor
    // --> from the electron closure
    // Field const& Pe = electrons.Pressure(/* Ni ? */ );

    // --> Get CORRECTED electric field E^{n+1} from Ohm's law
    // --> using (n^{n+1}, u^{n+1}) cor and B_{n+1} cor
    // ohm(B, Ne, Ve, Pe, E);

    // BC Fields --> Apply boundary conditions on the electric field
    //boundaryConditions_.applyElectricBC( E ) ;

}


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


void Solver::moveIons_(VecField const& E, VecField const& B, Ions& ions,
                       BoundaryCondition const * const boundaryCondition)
{


    // the temporary buffer must be big enough to hold the max
    // number of particles
    auto nbrParticlesMax = maxNbrParticles(ions);
    particleArrayPred_.reserve(nbrParticlesMax);


    for (uint32 ispe=0; ispe < ions.nbrSpecies(); ++ispe)
    {
        Species& species                 = ions.species(ispe);
        std::vector<Particle>& particles = species.particles();
        Interpolator& interpolator       = *interpolators_[ispe];


        // move all particles of that species from n to n+1
        // and put the advanced particles in the predictor buffer 'particleArrayPred_'
        pusher_->move(particles, particleArrayPred_, species.mass(), E, B, interpolator);

        // resize the buffer so that charge density and fluxes use
        // no more than the right number of particles
        // particleArrayPred_ has a capacity that is large enough for all
        // particle arrays for all species.
        particleArrayPred_.resize(particles.size());

        boundaryCondition->applyParticleBC(particleArrayPred_);
        computeChargeDensityAndFlux(interpolator, species, layout_, particleArrayPred_);

    } // end loop on species


    ions.computeChargeDensity();
    ions.computeBulkVelocity();

    boundaryCondition->applyDensityBC(ions.rho());
    boundaryCondition->applyBulkBC(ions.bulkVel());
}





