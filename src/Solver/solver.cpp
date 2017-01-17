

#include "Solver/solver.h"
#include "Interpolator/interpolator.h"
#include "Faraday/faradayfactory.h"
#include "pusher/pusherfactory.h"
#include "Field/field.h"
#include "Plasmas/ions.h"
#include "Plasmas/electrons.h"
#include "grid/gridlayout.h"



Solver::Solver( GridLayout const& layout, double dt,
                std::unique_ptr<SolverInitializer> solverInitializer )
    : EMFieldsPred_{{ {layout.allocSize(HybridQuantity::Ex ),
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
      faraday_{dt, layout}
{

    uint32 size = static_cast<uint32> ( solverInitializer->interpolationOrders.size() ) ;
    for( uint32 ik=0 ; ik<size ; ++ik )
    {
        uint32 order = solverInitializer->interpolationOrders[ik] ;
        interpolators_.push_back( std::unique_ptr<Interpolator>(new Interpolator(order)) ) ;
        //projector_.push_back( std::unique_ptr<Projector>(new Projector(order)) ) ;
    }

    const std::string pusherType = solverInitializer->pusherType ;

    pusher_ =  PusherFactory::createPusher( layout, pusherType ) ;


    // TODO need to initialize OHM object
    // TODO and vector (?) of particles (n+1)
    // TODO boundary conditions (?)
    // TODO projections/interpolations

}




void Solver::solveStep(Electromag& EMFields, Ions& ions, Electrons& electrons)
{
    VecField &B      = EMFields.getB();
    VecField &E      = EMFields.getE();

    VecField &Bpred  = EMFieldsPred_.getB();
    VecField &Epred  = EMFieldsPred_.getE();

    VecField &Bavg   = EMFieldsAvg_.getB();
    VecField &Eavg   = EMFieldsAvg_.getE();



    // --> Get B_{n+1} pred1 from E^n
    faraday_(E, B, Bpred);

    // --> MOMENTS (n^n, u^n) at time n have
    // --> already been computed, or are known just after initialization
    // --> Get ion and electron moments at time n

    // --> Get electric field E_{n+1} pred1 from Ohm's law
    // --> using (n^n, u^n) and B_{n+1}
    // ohm(Bpred, Ne, Ve, Pe, Epred);

    // BC Fields --> Apply boundary conditions on the electric field

    // --> Get time averaged prediction (E,B)_(n+1/2) pred1
    // --> using (E^n, B^n) and (E^{n+1}, B^{n+1}) pred1

    // VectorField::avg(E, Epred, Eavg);
    // VectorField::avg(B, Bpred, Bavg);

    // --> Move ions from n to n+1 using (E^{n+1/2},B^{n+1/2}) pred 1
    // moveIons(ions);

    // BC Parts --> Apply boundary conditions on particles

    // --> Get B^{n+1} pred2 from E^{n+1/2} pred1
    faraday_(Eavg, B, Bpred);


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

    // --> Get time averaged prediction (E^(n+1/2),B^(n+1/2)) pred2
    // --> using (E^n, B^n) and (E^{n+1}, B^{n+1}) pred2
    // VectorField::avg(E, Epred, Eavg);
    // VectorField::avg(B, Bpred, Bavg);

    // --> Get the CORRECTED positions and velocities
    // --> Move ions from n to n+1 using (E^{n+1/2},B^{n+1/2}) pred2
    // moveIons(ions);

    // BC Parts --> Apply boundary conditions on particles

    // --> Get CORRECTED B^{n+1} from E^{n+1/2} pred2
    // faraday(Eavg, B, B);

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

}





