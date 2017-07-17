
#include <algorithm>

#include "AMR/refinmentanalyser.h"




bool RefinementAnalyser::refinementNeeded(uint32 iter, uint32 iLevel, uint32 iPatch)
{
    bool result = false;

    auto range = std::equal_range(fakeStratIter_.begin(), fakeStratIter_.end(), iter);

    for (auto rk = range.first; rk != range.second; ++rk)
    {
        uint32 ik = std::distance(fakeStratIter_.begin(), rk);

        // auto ik_th = fakeStratLevel_.begin() + ik ;

        if (fakeStratLevel_[ik] == iLevel && fakeStratPatch_[ik] == iPatch)
        {
            result = true;
        }
    }


    return result;
}



bool RefinementAnalyser::refine(Patch const& patch)
{
    bool result = false;

    // analyze vecfields in data


    // if needed, build the refine box
    Box box{patch.layout().getBox()};

    Box refinedBox{box.x0 + 0.25 * (box.x1 - box.x0),
                   box.x0 + 0.75 * (box.x1 - box.x0),
                   box.y0,
                   box.y1,
                   box.z0,
                   box.z1};

    refinedVolumes_.push_back(refinedBox);

    return result;
}
