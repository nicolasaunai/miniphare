#include "hierarchy.h"

#include "AMR/patch.h"

#include "utility.h"


void Hierarchy::addNewPatch( Patch const & parent,
                             Box & position, uint32 level )
{



}


/**
 * @brief Hierarchy::evolveHierarchy
 *
 * evolve fields and particle for a time step
 *
 */
void Hierarchy::evolveHierarchy()
{

    uint32 nbrLevels = static_cast<uint32>(patchTable_.size()) ;

    for( uint32 iLevel=0 ; iLevel<nbrLevels ; iLevel++ )
    {
        auto & patchesAtLevel = patchTable_[iLevel] ;

        for( std::shared_ptr<Patch> patch: patchesAtLevel)
        {
            patch->evolve() ;
        }

    }

}


/**
 * @brief Hierarchy::evaluateHierarchy
 *
 * Here, AMR patches will say whether they need refinement
 * the ouput of this method is used by updateHierarchy()
 *
 */
void Hierarchy::evaluateHierarchy()
{

    uint32 nbrLevels = static_cast<uint32>(patchTable_.size()) ;

    for( uint32 iLevel=0 ; iLevel<nbrLevels ; iLevel++ )
    {
        auto & patchesAtLevel = patchTable_[iLevel] ;

        for( std::shared_ptr<Patch> patch: patchesAtLevel)
        {
            RefinementAnalyser analyser{} ;

            patch->checkRefinment( analyser ) ;
        }

    }


}


/**
 * @brief Hierarchy::updateHierarchy
 *
 * new patches are created here if necessary
 * it depends on evaluateHierarchy()
 *
 */
void Hierarchy::updateHierarchy()
{


}


