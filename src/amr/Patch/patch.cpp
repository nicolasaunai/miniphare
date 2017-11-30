#include "patch.h"
#include "utilities/print/outputs.h"


void Patch::init()
{
    Logger::Debug << "Initialize patch data\n";
    Logger::Debug.flush();

    data_.initPatchPhysicalDomain();

    Logger::Debug << "Patch initialization OK\n";
    Logger::Debug.flush();
}




void Patch::addChild(std::shared_ptr<Patch> newChild)
{
    children_.push_back(newChild);
}
