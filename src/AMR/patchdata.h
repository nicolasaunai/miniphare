#ifndef PATCHDATA_H
#define PATCHDATA_H



#include "Solver/solver.h"
#include "Plasmas/ions.h"
#include "Plasmas/electrons.h"
#include "Electromag/electromag.h"
#include "grid/gridlayout.h"
#include "Initializer/initializerfactory.h"
#include "types.h"


/**
 * @brief The PatchData class encapsulate data for a Patch necessary for
 * the Solver to solve the hybrid system of equations.
 * A PatchData is a heavy object since it contains all Ions etc. Therefore
 * it is a move only object.
 */
class PatchData
{
private:

    Electromag EMfields_;
    Solver     solver_;
    Ions ions_;
    //Electrons electrons_;


public:

    PatchData(std::unique_ptr<InitializerFactory> initFactory);

   // PatchData(double dt, const GridLayout& layout);

    PatchData(PatchData const& source) = delete;
    PatchData& operator=(PatchData const& source) = delete;

    PatchData(PatchData&& source) = default;
    PatchData& operator=(PatchData&& source) = default;

    void init();

    Ions const& ions() const {return ions_;}

    //void advance(){solver_.solveStep(EMfields_, ions_, electrons_);}

};

#endif // PATCHDATA_H




