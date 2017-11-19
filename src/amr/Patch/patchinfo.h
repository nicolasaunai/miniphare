#ifndef PATCHINFO_H
#define PATCHINFO_H

#include <string>
#include <vector>

#include "utilities/types.h"


struct PatchInfo
{
    std::string pusher;
    uint32 interpOrder;
    uint32 refinementRatio;
    std::vector<std::string> splitStrategies;

    double userTimeStep; // base L0 time step

    // this will help debug and tests
    // for MLMD feature
    std::vector<uint32> fakeStratIteration;
    std::vector<uint32> fakeStratLevelToRefine;
    std::vector<uint32> fakeStratPatchToRefine;
};


#endif // PATCHINFO_H
