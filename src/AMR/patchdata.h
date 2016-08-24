#ifndef PATCHDATA_H
#define PATCHDATA_H



#include "Solver/solver.h"
#include "Plasmas/ions.h"
#include "Plasmas/electrons.h"
#include "Electromag/electromag.h"
#include "grid/gridlayout.h"

#include "types.h"


class PatchData
{
private:

    Electromag EMfields_;
    Solver     solver_;
    //Ions ions_;
    //Electrons electrons_;


public:
    PatchData(double dt, const GridLayout& layout);
    //void advance(){solver_.solveStep(EMfields_, ions_, electrons_);}

};

#endif // PATCHDATA_H




