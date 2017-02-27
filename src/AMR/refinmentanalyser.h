#ifndef REFINMENTANALYSER_H
#define REFINMENTANALYSER_H

#include "AMR/patchdata.h"

#include "utilityphare.h"




class RefinementAnalyser
{
private:
    // it tells where the mesh should be refined
    // Initialize with an empty Box
    Box refinedArea_ ;

public:

    RefinementAnalyser()
        : refinedArea_{ 0, 0, 0, 0, 0, 0} {}

    bool operator()(PatchData const& data)
    {
        bool result = false ;

        // analyze vecfields in data


        // if needed, build the refine box


        return result ;
    }

    Box refinedArea() { return refinedArea_; }

    bool hasNoEmptyBox() ;
};



#endif // REFINMENTANALYSER_H
