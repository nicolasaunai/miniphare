#ifndef SOLVER_H
#define SOLVER_H

#include <memory>
#include <array>
#include <vector>

#include "types.h"
#include "Ohm/ohm.h"
#include "Plasmas/ions.h"
#include "Ampere/ampere.h"
#include "pusher/pusher.h"
#include "grid/gridlayout.h"
#include "Faraday/faraday.h"
#include "Plasmas/electrons.h"
#include "Electromag/electromag.h"
#include "Interpolator/interpolator.h"
#include "Initializer/solverinitializer.h"
#include "BoundaryConditions/boundary_conditions.h"


class Solver
{

public:

    Solver( GridLayout const& layout, double dt, \
            std::unique_ptr<SolverInitializer> solverInitializer );

    Solver(Solver const& source) = delete;
    Solver& operator=(Solver const& source) = delete;

    Solver(Solver&& toMove)      = default;
    Solver& operator=(Solver&& source) = default;

    void solveStep(Electromag& EMFields, Ions& ions, Electrons& electrons,
                   BoundaryCondition const * const boundaryCondition);

private:

    const uint32 predictor1_ = 0;
    const uint32 predictor2_ = 1;

    GridLayout layout_;

    // quantities used in the solver
    // these are temporary quantities to use within a time step
    Electromag EMFieldsPred_ ;
    Electromag EMFieldsAvg_ ;
    VecField Jtot_ ;
    std::vector<Particle> particleArrayPred_;




    // algorithms
    Faraday faraday_;
    Ampere  ampere_;
    Ohm ohm_;
    std::vector< std::unique_ptr<Interpolator> > interpolators_ ;
    std::unique_ptr<Pusher> pusher_ ;

    void moveIons_(VecField const& E, VecField const& B, Ions& ions,
                   BoundaryCondition const * const boundaryConditon,
                   uint32 const predictorStep);

};



#endif // SOLVER_H




