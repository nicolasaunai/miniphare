#ifndef PATCHINFO_H
#define PATCHINFO_H

#include <string>
#include <vector>

#include "utilities/types.h"


struct PatchInfos
{
    std::string pusher;
    uint32 interpOrder;
    uint32 refinementRatio;
    std::vector<std::string> splitStrategies;

    double userTimeStep; // base L0 time step
};


#endif // PATCHINFO_H
