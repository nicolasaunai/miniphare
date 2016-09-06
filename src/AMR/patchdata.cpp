


#include "patchdata.h"


// contains all hybrid patch stuff (ions, fields, solver, etc.)


PatchData::PatchData(double dt, const GridLayout& layout)
    : EMfields_{layout.allocSize( EMFieldType::EVecField ),
                layout.allocSize( EMFieldType::BVecField ), "_currentEMfield"},
      solver_{layout, dt}
{}
