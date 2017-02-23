#include "hierarchy.h"

#include "AMR/patch.h"

#include "AMR/mlmdinitializerfactory.h"

#include "utilityphare.h"




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

            // TODO: return some structure with information
            // about where we should refine
            // TODO: evaluateHierarchy() should return this structure
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
 * Calls addNewPatch if necessary
 *
 */
// TODO: Add an argument
// TODO: Use the structure returned by evaluateHierarchy()
void Hierarchy::updateHierarchy( uint32 refinementRatio )
{

    // Go through the structure, to look where refinement
    // is needed

    // Whenever refinement is needed,
    // trigger refinement with a call to addNewPatch(...)
//    addNewPatch( parent, newPatch,
//                 refinement, level ) ;


}



void Hierarchy::addNewPatch( std::shared_ptr<Patch> parent,
                             Box & newPatch, uint32 refinement,
                             uint32 level )
{
    // we need to build a factory for PatchData to be built
    MLMDInitializerFactory factory(parent, newPatch, refinement) ;

    // create the new patch, give it a PatchData to which we pass the factory
//    Patch theNewPatch{ PatchData{ factory }};

    // somehow attach this new patch to the hierarchy...



}


