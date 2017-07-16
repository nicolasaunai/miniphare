#include "patch.h"



void Patch::init()
{
    std::cout << "init Patch...";
    data_.initPatchPhysicalDomain();
    std::cout << " Patch initialization: OK\n";
}



void Patch::solveStep()
{
    data_.solveStep();
}


uint32 Patch::population() const
{
    return data_.population();
}




void Patch::updateChildren(std::shared_ptr<Patch> newChild)
{
    children_.push_back(newChild);
}
