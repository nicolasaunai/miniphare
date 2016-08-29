#ifndef MLMD_H
#define MLMD_H

#include "grid/gridlayout.h"

/**
 * @brief The MLMD class deals with all MLMD operations.
 *
 * MLMD is the object executes MLDM operations:
 *     - dealing with PRA (splitting, inter patch communications, field interpolations etc.)
 *     - refinement / patch creation and insertion into the Hierarchy
 *     -  etc.
 */
class MLMD // TODO should be a singleton pattern
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
