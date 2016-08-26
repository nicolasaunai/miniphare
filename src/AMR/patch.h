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


    explicit Patch(PatchData&& patchData):data_{std::move(patchData)}{}
    //Patch(PatchData& patchData):data_{std::move(data)}{}

    Patch(Patch&& source) = default;
    Patch& operator=(Patch&& source) = default;

    Patch(Patch const& source) = delete;
    Patch& operator=(Patch& source) = delete;


};

#endif // PATCH_H
