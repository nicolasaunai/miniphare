#ifndef MLMDINFO_H
#define MLMDINFO_H

#include <string>
#include <vector>

#include "utilities/types.h"


struct MLMDInfos
{
    // Size control parameters when a new patch is created by MLMD
    // Example: minRatio = 0.4; maxRatio = 0.6
    //
    // |-----------------------------------------------------------|      PARENT PATCH
    //
    //                   |--------------------|                           CHILD PATCH
    //
    double minRatio;
    double maxRatio;

    // MLMD refinement strategy
    std::vector<uint32> refineIterations;
    std::vector<uint32> levelsToRefine;
    std::vector<uint32> patchToRefine;
};


#endif // MLMDINFO_H
