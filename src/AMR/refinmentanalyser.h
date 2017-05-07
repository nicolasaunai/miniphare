#ifndef REFINMENTANALYSER_H
#define REFINMENTANALYSER_H

#include "AMR/patchdata.h"

#include "utilityphare.h"




class RefinementAnalyser
{
private:
    // it tells where the mesh should be refined
    // Initialize with an empty Box
    std::vector<Box> refinedVolumes_;

public:
    RefinementAnalyser()
        : refinedVolumes_{{0, 0, 0, 0, 0, 0}}
    {
    }

    bool operator()(PatchData const& data)
    {
        bool result = false;

        // analyze vecfields in data


        // if needed, build the refine box


        return result;
    }

    std::vector<Box> const& refinedDomains() { return refinedVolumes_; }

    bool refinementNeeded();
};



#endif // REFINMENTANALYSER_H
