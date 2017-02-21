#ifndef PATCH_H
#define PATCH_H


// data structure Tree

#include "AMR/patchdata.h"
#include "Plasmas/ions.h"
#include "utility.h"


/**
 * @brief Represents a "node" in the patch Hierarchy.
 *
 * Patch holds a pointer to its (single) parent and to its children.
 * A Patch knows nothing about the data it encapsulates.
 * A Patch is a move-only object. Only two modules can create Patches:
 *      - the initial condition module, to initialize the simulation
 *      - the MLMD module, when refinement is needed.
 *
 *  Both modules will create temporary Patch object and std::move() them
 *  to a Hierarchy. Copy is not allowed as a Patch encapsulate potentially
 *  heavy PatchData object, which is also move-only for the same reason.
 */
class Patch
{

private:

    Box coordinates_;
    PatchData data_;
    // parent pointer
    // vector of children
    //

public:

    explicit Patch(PatchData&& patchData):data_{std::move(patchData)}{}

    Patch(Patch&& source) = default;
    Patch& operator=(Patch&& source) = default;


    Patch(Patch const& source) = delete;
    Patch& operator=(Patch& source) = delete;

    void init() { std::cout << "init Patch...";  data_.init(); std::cout << " patch initialized OK\n";}

    Ions const& ions() const { return data_.ions(); }

    Box coordinates() const { return coordinates_; }

};

#endif // PATCH_H





