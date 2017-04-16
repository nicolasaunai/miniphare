#ifndef PATCHINFO_H
#define PATCHINFO_H

#include<string>
#include<vector>

#include "types.h"

struct PatchInfo
{
    std::string pusher;
    std::vector<uint32> interpOrders;
    uint32 refinementRatio;
    std::vector<std::string> splitStrategies;
};


#endif // PATCHINFO_H
