#include "patch.h"

bool Patch::checkRefinment( RefinementAnalyser const & analyser ) const
{
    bool result = false ;



    return result;
}


void Patch::evolve()
{

    data_.solveStep() ;

}



void Patch::updateChildren( std::shared_ptr<Patch> newChild )
{

    children_.push_back( newChild ) ;
}

