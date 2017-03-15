
#include <tuple>
#include <memory>

#include "grid/gridlayout.h"

#include "AMR/hierarchy.h"
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
    uint32 nbrLevels = static_cast<uint32>(patchTable_.size());

    // TODO need to include all interactions with children
    // https://hephaistos.lpp.polytechnique.fr/redmine/projects/hyb-par/wiki/BoundaryConditions#MLMD-time-integration

    for( uint32 iLevel=0 ; iLevel<nbrLevels ; iLevel++ )
    {
        uint32 nbrPatches = static_cast<uint32>(patchTable_[iLevel].size());

        for (uint32 iPatch=0; iPatch < nbrPatches; ++iPatch)
        {
            patchTable_[iLevel][iPatch]->evolve();
        }
    } // end loop on levels
}


/**
 * @brief Hierarchy::evaluateHierarchy
 *
 * Here, AMR patches will say whether they need refinement
 * the ouput of this method is used by updateHierarchy()
 *
 */
std::vector< std::vector<RefinementInfo> >
Hierarchy::evaluateHierarchy()
{

    std::vector< std::vector<RefinementInfo> > refinementTable ;

    uint32 nbrLevels = static_cast<uint32>(patchTable_.size()) ;

    for( uint32 iLevel=0 ; iLevel<nbrLevels ; iLevel++ )
    {
        auto & patchesAtLevel = patchTable_[iLevel] ;

        std::vector<RefinementInfo> refinementVector ;

        for( std::shared_ptr<Patch> patch: patchesAtLevel)
        {
            RefinementAnalyser analyser{} ;

            analyser( patch->data() ) ;

            // if the patch has to be refined we store a reference
            // for further use
            if( analyser.hasNoEmptyBox() )
            {
                Box refineBox = analyser.refinedArea() ;

                struct RefinementInfo refine{ patch, refineBox, iLevel+1 } ;

                refinementVector.push_back( refine ) ;
            }
        }

        refinementTable.push_back( std::move(refinementVector) ) ;
    }

    return refinementTable ;
}



/**
 * @brief Hierarchy::updateHierarchy
 *
 * New patches are created here depending on the content
 * of refinementTable (and layoutTable)
 * These two tables have the same number of elements
 *
 * @param refinementTable
 * @param layoutTable
 */
void Hierarchy::updateHierarchy(
        std::vector< std::vector<RefinementInfo> > const & refinementTable,
        std::vector< std::vector<GridLayout> > const & layoutTable  )
{

    uint32 nbrLevels = static_cast<uint32>(refinementTable.size()) ;

    for( uint32 iLevel=0 ; iLevel<nbrLevels ; ++iLevel )
    {
        uint32 nbrPatches = static_cast<uint32>( refinementTable[iLevel].size() ) ;

        for( uint32 iPatch=0 ; iPatch<nbrPatches ; ++iPatch )
        {
            GridLayout const & layout = layoutTable[iLevel][iPatch] ;
            RefinementInfo const & info = refinementTable[iLevel][iPatch] ;

            // create new Patch and update Hierarchy
            addNewPatch( layout, info ) ;

            // TODO: call patch.init to initialize patch content


        }
    }

}



void Hierarchy::addNewPatch( GridLayout const & refinedLayout,
                             RefinementInfo const & info)
{

    std::shared_ptr<Patch> coarsePatch = info.parentPatch ;
    Box refinedBox = info.box ;
    uint32 refinedLevel = info.level ;

    // we need to build a factory for PatchData to be built
    std::unique_ptr<InitializerFactory>
            factory { new MLMDInitializerFactory(coarsePatch, refinedBox, refinedLayout) } ;

    // create the new patch, give it a PatchData to which we pass the factory
    Patch theNewPatch{ refinedBox, refinedLayout, PatchData{ factory.get() } };

    std::shared_ptr<Patch> patchPtr = std::make_shared<Patch>( std::move(theNewPatch) ) ;

    // somehow attach this new patch to the hierarchy...
    coarsePatch->updateChildren( patchPtr ) ;

    // update the hierarchy
    patchTable_[refinedLevel].push_back( patchPtr ) ;

}








