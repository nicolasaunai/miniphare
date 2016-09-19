#ifndef HIERARCHY_H
#define HIERARCHY_H

#include "AMR/patch.h"


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
