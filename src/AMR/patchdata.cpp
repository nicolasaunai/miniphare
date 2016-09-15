


#include "patchdata.h"
#include "hybridenums.h"

// contains all hybrid patch stuff (ions, fields, solver, etc.)


PatchData::PatchData(double dt, const GridLayout& layout)
    : EMfields_{ { {layout.allocSize(HybridQuantity::Ex ),
                  layout.allocSize(HybridQuantity::Ey ),
                  layout.allocSize(HybridQuantity::Ez )  }},

                  { {layout.allocSize(HybridQuantity::Bx ),
                  layout.allocSize(HybridQuantity::By ),
                  layout.allocSize(HybridQuantity::Bz )  }},

                 "_currentEMfield"},
        solver_{layout, dt}
{}
