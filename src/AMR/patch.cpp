#include "patch.h"



void Patch::init()
{
    std::cout << "init Patch...";
    data_.initPatchPhysicalDomain();
    std::cout << " Patch initialization: OK\n" ;

    data_.initPatchGhostDomain();
    std::cout << " PRA initialization: OK\n" ;
}



void Patch::evolve()
{

    data_.solveStep() ;

}



void Patch::updateChildren( std::shared_ptr<Patch> newChild )
{
    children_.push_back( newChild ) ;
}

