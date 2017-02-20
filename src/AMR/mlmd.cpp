
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


/**
 * @brief MLMD::evolveHierarchy
 *
 * evolve fields and particle for a time step
 *
 */
void MLMD::evolveHierarchy()
{

    std::vector< std::vector< std::shared_ptr<Patch> > >
            patchArray = patchHierarchy_.patchTable() ;

    uint32 nbrLevels = static_cast<uint32>(patchArray.size()) ;

    for( uint32 iLevel=0 ; iLevel<nbrLevels ; iLevel++ )
    {
        auto & patchesAtLevel = patchArray[iLevel] ;

        for( std::shared_ptr<Patch> patch: patchesAtLevel)
        {
            patch->evolve() ;
        }

    }

}


/**
 * @brief MLMD::evaluateHierarchy
 *
 * Here, AMR patches will say whether they need refinement
 * the ouput of this method is used by updateHierarchy()
 *
 */
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


/**
 * @brief MLMD::updateHierarchy
 *
 * new patches are created here if necessary
 * it depends on evaluateHierarchy()
 *
 */
void MLMD::updateHierarchy()
{


}


