#ifndef SOLVER_H
#define SOLVER_H

#include <array>
#include <memory>
#include <vector>

#include "core/Ampere/ampere.h"
#include "core/BoundaryConditions/boundary_conditions.h"
#include "core/Faraday/faraday.h"
#include "core/Interpolator/interpolator.h"
#include "core/Ohm/ohm.h"
#include "core/pusher/pusher.h"

#include "initializer/solverinitializer.h"

#include "data/Electromag/electromag.h"
#include "data/grid/gridlayout.h"

#include "utilities/types.h"

class Ions;
class Electrons;


class Solver
{
private:
    const uint32 predictor1_ = 0;
    const uint32 predictor2_ = 1;

    GridLayout layout_;

    // quantities used in the solver
    // these are temporary quantities to use within a time step
    Electromag EMFieldsPred_;
    Electromag EMFieldsAvg_;
    VecField Jtot_;
    std::vector<Particle> particleArrayPred_;


    // algorithms
    Faraday faraday_;
    Ampere ampere_;
    Ohm ohm_;
    Interpolator interpolator_;
    std::unique_ptr<Pusher> pusher_;

    void moveIons_(VecField const& E, VecField const& B, Ions& ions,
                   BoundaryCondition& boundaryConditon, uint32 const predictorStep);

public:
    Solver(GridLayout const& layout, double dt,
           std::unique_ptr<SolverInitializer> solverInitializer);

    Solver(Solver const& source) = delete;
    Solver& operator=(Solver const& source) = delete;

    Solver(Solver&& toMove) = default;
    Solver& operator=(Solver&& source) = default;

    void init(Ions& ions, BoundaryCondition const& boundaryCondition);

    void solveStepPPC(Electromag& EMFields, Ions& ions, Electrons& electrons,
                      BoundaryCondition& boundaryCondition);
};



#endif // SOLVER_H
