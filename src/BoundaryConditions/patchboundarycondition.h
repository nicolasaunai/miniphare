#ifndef PATCHBOUNDARYCONDITION_H
#define PATCHBOUNDARYCONDITION_H


#include "Plasmas/ions.h"
#include "Electromag/electromag.h"
#include "Initializer/initializerfactory.h"


class PatchBoundaryCondition
{

private:
    Electromag EMfields_;
    Ions ions_;


public:
    PatchBoundaryCondition(std::unique_ptr<InitializerFactory> initFactory);
};

#endif // PATCHBOUNDARYCONDITION_H
