#include "patch.h"



void Patch::init()
{
    std::cout << "init Patch...";
    data_.initPatchPhysicalDomain();
    std::cout << " Patch initialization: OK\n";
}




void Patch::addChild(std::shared_ptr<Patch> newChild)
{
    children_.push_back(newChild);
}
