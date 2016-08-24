#ifndef PATCH_H
#define PATCH_H


// data structure Tree

#include "AMR/patchdata.h"


class Patch
{

private:

    PatchData data_;

    // parent pointer
    // vector of children
    //

public:
    explicit Patch(PatchData& data):data_{data}{}


};

#endif // PATCH_H
