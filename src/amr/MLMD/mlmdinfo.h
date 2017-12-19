#ifndef MLMDINFO_H
#define MLMDINFO_H

#include <string>
#include <vector>

#include "utilities/types.h"


struct MLMDInfos
{
    // Size control parameters when a new patch is created by MLMD
    double minRatio;
    double maxRatio;

    // MLMD refinement strategy
    std::vector<uint32> fakeStratIteration;
    std::vector<uint32> fakeStratLevelToRefine;
    std::vector<uint32> fakeStratPatchToRefine;
};


#endif // MLMDINFO_H
