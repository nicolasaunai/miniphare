#ifndef MLMD_H
#define MLMD_H

#include "grid/gridlayout.h"

class MLMD
{

private:
    uint32 refinementRatio{2};
    // PRAwidthx,
    // splitting

   // TODO GridLayout* generateLayoutFromRefineRatio() ....
   // TODO void computedtFromCFL(); // calculate dt from CFL and dx_ // for MLMD version

public:
    MLMD();
};

#endif // MLMD_H
