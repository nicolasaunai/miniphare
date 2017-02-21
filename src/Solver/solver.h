#ifndef SOLVER_H
#define SOLVER_H

#include <memory>
#include <array>
#include <vector>

#include "types.h"
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

    GridLayout layout_;

    // quantities used in the solver
    // these are temporary quantities to use within a time step
    Electromag EMFieldsPred_ ;
    Electromag EMFieldsAvg_ ;
    VecField Jtot_ ;
    std::vector<Particle> particleArrayPred_;// TODO build this

    // algorithms
    Faraday faraday_;
    Ampere  ampere_;
    std::vector< std::unique_ptr<Interpolator> > interpolators_ ;
    std::unique_ptr<Pusher> pusher_ ;


    // methods
    void moveIons_(VecField const& E, VecField const& B, Ions& ions,
                   BoundaryCondition const * const boundaryConditon);
};



#endif // SOLVER_H




