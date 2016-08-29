


#include "patchdata.h"


// contains all hybrid patch stuff (ions, fields, solver, etc.)


PatchData::PatchData(double dt, const GridLayout& layout)
    : EMfields_{layout.nx(),layout.ny(),layout.nz(), "_currentEMfield"},
      solver_{layout, dt}
{}
