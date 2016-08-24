

#include "Solver/solver.h"
#include "Faraday/faradayfactory.h"
#include "Field/field.h"
#include "Plasmas/ions.h"
#include "Plasmas/electrons.h"
#include "grid/gridlayout.h"



Solver::Solver(GridLayout const& layout, double dt)
    :dt_{dt}, layout_{layout},
     EMFieldsPred_{layout.nx(), layout.ny(), layout.nz(), "_pred"},
     EMFieldsAvg_ {layout.nx(), layout.ny(), layout.nz(), "_avg"}
{

    std::vector<double>dxdydz{layout.dx(), layout.dy(), layout.dz()};

    auto nbDims = layout.nbDimensions();

    faradaySolver_ = FaradayFactory::createFaradaySolver(dt, dxdydz, nbDims);


    // TODO need to initialize OHM object
    // TODO and vector (?) of particles (n+1)
    // TODO boundary conditions (?)
    // TODO projections/interpolations

}



Solver::Solver(Solver const& source)
    : dt_{source.dt_}, layout_{source.layout_},
      EMFieldsAvg_ { source.EMFieldsAvg_ },
      EMFieldsPred_{ source.EMFieldsPred_},
      faradaySolver_{source.faradaySolver_->clone()}

{

}







void Solver::solveStep(Electromag& EMFields, Ions& ions, Electrons& electrons)
{
    VecField &B      = EMFields.getB();
    VecField &E      = EMFields.getE();

    VecField &Bpred  = EMFieldsPred_.getB();
    VecField &Epred  = EMFieldsPred_.getE();

    VecField &Bavg   = EMFieldsAvg_.getB();
    VecField &Eavg   = EMFieldsAvg_.getE();

    Faraday& faraday = *faradaySolver_;
    // Ohm& ohm        = *ohmSolver_;

    // B_{n+1} pred 1
    faraday(E, B, Bpred);


    // get ion and electron moments at time n
    // Field const& Ni = ions.chargeDensity();
    // VecField const& Vi = ions.bulkVelocity();
    // Field const& Pe = electrons.Pressure(/* Ni ? */ );
    // VecField const& Ve = electrons.bulkVelocity(B, Vi, Ni);


    // E_{n+1} pred 1
    // ohm(Bpred, Ve, Pe, Epred);

    // (E,B)_(n+1/2) pred 1

    // VectorField::avg(E, Epred, Eavg);
    // VectorField::avg(B, Bpred, Bavg);

    // move ions from n to n+1 using (E,B)_(n+1/2) pred 1
    // moveIons(ions);

    //  // part2_ is invalid

    // B_{n+1} Pred 2
    faraday(Eavg, B, Bpred);

    // get ion and electron moments at time n+1 (pred 1)
    // Field const& Ni = ions.chargeDensity();
    // VecField const& Vi = ions.bulkVelocity();
    // Field const& Pe = electrons.Pressure(/* Ni ? */ );
    // VecField const& Ve = electrons.bulkVelocity(B, Vi, Ni);

    // E_{n+1} pred 2
    // ohm(Bpred, Ve, Pe, Epred);

    // (E,B)_(n+1/2) pred 2
    // VectorField::avg(E, Epred, Eavg);
    // VectorField::avg(B, Bpred, Bavg);

     // move ions from n to n+1 using (E,B)_(n+1/2) pred 2
    // moveIons(ions);

    // B_{n+1} Corr. from E_(n+1/2) and Bn
    // faraday(Eavg, B, B); // B and B artung????

    // get ion and electron moments at time n+1 (pred 2)

    // Field const& Ni = ions.chargeDensity();
    // VecField const& Vi = ions.bulkVelocity();
    // Field const& Pe = electrons.Pressure(/* Ni ? */ );
    // VecField const& Ve = electrons.bulkVelocity(B, Vi, Ni);

    // E_{n+1} pred 2
    // ohm(B, Ve, Pe, E);

}





