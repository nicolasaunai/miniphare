#ifndef SOLVER_H
#define SOLVER_H

#include <memory>
#include <array>
#include <vector>

#include "types.h"
#include "Initializer/solverinitializer.h"
#include "Plasmas/ions.h"
#include "Plasmas/electrons.h"
#include "Faraday/faraday.h"
#include "Ampere/ampere.h"
#include "Electromag/electromag.h"
#include "grid/gridlayout.h"
#include "BoundaryConditions/fieldbc3dcollection.h"
#include "Interpolator/interpolator.h"
#include "pusher/pusher.h"


class Solver
{

public:

    Solver( GridLayout const& layout, double dt, \
            std::unique_ptr<SolverInitializer> solverInitializer );

    Solver(Solver const& source) = delete;
    Solver& operator=(Solver const& source) = delete;

    Solver(Solver&& toMove)      = default;
    Solver& operator=(Solver&& source) = default;

    void solveStep(Electromag& EMFields, Ions& ions, Electrons& electrons);


private:

    // those are Solver attribute because SOLVER decides where interp/project
    // work on the mesh.... a different solver may interp/project elsewhere
    std::vector< std::unique_ptr<Interpolator> > interpolators_ ;

    Electromag EMFieldsPred_ ;
    Electromag EMFieldsAvg_ ;

    VecField Jtot_ ;

    // vector <particle> part2_;  // vector of vector ou assign.

    std::unique_ptr<Pusher> pusher_ ;

    Faraday faraday_;
    Ampere  ampere_;

    // ohm object

    CollectionOfBoundaryConditions boundaryConditions_ ;





    /*
    move_(ions)
    {
        for (int iSpecies=0; iSpecies < ions.nbSpecies(); ++iSpecies)
        {
            vector<Particle>const& part1 = spe.particles();
            moveParticle(part1, part2_);
            projector_[iSpecies](part2_, ions.species(iSpecies));
        }

    }*/


};

#endif // SOLVER_H




