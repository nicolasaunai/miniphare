#ifndef REFINMENTANALYSER_H
#define REFINMENTANALYSER_H

#include "amr/Patch/patch.h"
#include "utilities/box.h"




class RefinementAnalyser
{
private:
    // it tells where the mesh should be refined
    // Initialize with an empty Box
    std::vector<Box> refinedVolumes_;

    std::vector<uint32> const fakeStratIter_;
    std::vector<uint32> const fakeStratLevel_;
    std::vector<uint32> const fakeStratPatch_;

public:
    RefinementAnalyser(std::vector<uint32> const& fakeStratIter,
                       std::vector<uint32> const& fakeStratLevel,
                       std::vector<uint32> const& fakeStratPatch)
        : fakeStratIter_{fakeStratIter}
        , fakeStratLevel_{fakeStratLevel}
        , fakeStratPatch_{fakeStratPatch}
    {
    }

    bool refinementNeeded(uint32 iter, uint32 iLevel, uint32 iPatch);

    bool refine(Patch const& patch);

    std::vector<Box> const& refinedDomains() { return refinedVolumes_; }
};



#endif // REFINMENTANALYSER_H
