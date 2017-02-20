
#include "mlmd.h"
#include "hierarchy.h"
#include "patch.h"
#include "patchdata.h"
#include "Solver/solver.h"


MLMD::MLMD(std::unique_ptr<InitializerFactory> initFactory)
    : patchHierarchy_{ Patch { PatchData{ std::move(initFactory) }  } }
{
    // will probably have to change the way objects are initialized.
    // if we want, at some point, start from an already existing hierarchy
    // (in case of restart for e.g.
}



void MLMD::initializeRootLevel()
{
    Patch& rootLevel = patchHierarchy_.root();
    std::cout << "root level OK" << std::endl;
    rootLevel.init();

}


void MLMD::evolveHierarchy()
{

}


void MLMD::evaluateHierarchy()
{

    std::vector< std::vector< std::shared_ptr<Patch> > >
            patchArray = patchHierarchy_.patchTable() ;

    uint32 nbrLevels = static_cast<uint32>(patchArray.size()) ;

    for( uint32 iLevel=0 ; iLevel<nbrLevels ; iLevel++ )
    {
        auto & patchesAtLevel = patchArray[iLevel] ;

        for( std::shared_ptr<Patch> patch: patchesAtLevel)
        {
            RefinementAnalyser analyser{} ;

            patch->checkRefinment( analyser ) ;
        }

    }


}


void MLMD::updateHierarchy()
{


}


