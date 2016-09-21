#ifndef HIERARCHY_H
#define HIERARCHY_H

#include "AMR/patch.h"

/**
 * @brief The Hierarchy class describes the hierarchy of Patches.
 * It ignores what's in Patches, just knows how to go from one patch to its
 * children and the other way.
 */
class Hierarchy
{

private:

        Patch root_;

public:


    explicit Hierarchy(Patch&& root):root_{std::move(root)} { /*root_->parent = nullptr;*/}
    //Hierarchy(Patch&& root):root_{}{}


    Patch& root() { return root_; }




};

#endif // HIERARCHY_H
