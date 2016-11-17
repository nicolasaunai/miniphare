#ifndef SOLVER_H
#define SOLVER_H

#include <memory>
#include <vector>

#include "types.h"
#include "Plasmas/ions.h"
#include "Plasmas/electrons.h"
#include "Faraday/faraday.h"
#include "Electromag/electromag.h"
#include "grid/gridlayout.h"

#include "pusher/pusher.h"


class Solver
{

public:

    Solver( const std::string & pusherType, GridLayout const& layout,
            double dt );

    Solver(Solver const& source) = delete;
    Solver& operator=(Solver const& source) = delete;

    Solver(Solver&& toMove)      = default;
    Solver& operator=(Solver&& source) = default;

    void solveStep(Electromag& EMFields, Ions& ions, Electrons& electrons);


private:

    // TODO
    // projector(); // depositSpecies
    // deposit();
    // those are Solver attribute because SOLVER decides where interp/project
    // work on the mesh.... a different solver may interp/project elsewhere
    // (is this really true??)


    //std::vector< std::unique_ptr<Projector> > projector; // is of size Ion.nbSpecies();

    Electromag EMFieldsPred_;
    Electromag EMFieldsAvg_;
    // vector <particle> part2_;  // vector of vector ou assign.

    std::unique_ptr<Pusher> pusher_ ;


    // BoundaryCondition bc_;

    Faraday faraday_;
    // ohm object

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




