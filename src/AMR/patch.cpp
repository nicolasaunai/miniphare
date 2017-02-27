#include "patch.h"



void Patch::evolve()
{

    data_.solveStep() ;

}



void Patch::updateChildren( std::shared_ptr<Patch> newChild )
{

    children_.push_back( newChild ) ;
}

