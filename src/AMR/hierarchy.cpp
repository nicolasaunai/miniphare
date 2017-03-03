
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
std::vector<RefinementInfo>
Hierarchy::evaluateHierarchy()
{

    std::vector<RefinementInfo> patchToBeRefined{} ;

    uint32 nbrLevels = static_cast<uint32>(patchTable_.size()) ;

    for( uint32 iLevel=0 ; iLevel<nbrLevels ; iLevel++ )
    {
        auto & patchesAtLevel = patchTable_[iLevel] ;

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

                patchToBeRefined.push_back( refine ) ;
            }
        }

    }

    return patchToBeRefined ;
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
void Hierarchy::updateHierarchy( std::vector<GridLayout> const & newLayouts,
                                 std::vector<RefinementInfo> const & refineInfo )
{

    uint32 nbrPatches = static_cast<uint32>(newLayouts.size()) ;

    // Go through the new layouts,
    // a patch is added when required
    for( uint32 iPatch=0 ; iPatch<nbrPatches ; ++iPatch )
    {
        GridLayout const & layout = newLayouts[iPatch] ;
        RefinementInfo const & info = refineInfo[iPatch] ;

        // Whenever refinement is needed,
        // trigger refinement
        addNewPatch( layout, info ) ;

        // TODO: call patch.init to initialize patch content


    }


}



void Hierarchy::addNewPatch( GridLayout const & refinedLayout,
                             RefinementInfo const & info)
{

    std::shared_ptr<Patch> coarsePatch = info.parentPatch ;
    Box refinedBox = info.box ;

    // we need to build a factory for PatchData to be built
    std::unique_ptr<InitializerFactory>
            factory { new MLMDInitializerFactory(coarsePatch, refinedBox, refinedLayout) } ;

    // create the new patch, give it a PatchData to which we pass the factory
    Patch theNewPatch{ refinedBox, PatchData{ std::move(factory) } };

    // somehow attach this new patch to the hierarchy...
    coarsePatch->updateChildren( std::make_shared<Patch>( std::move(theNewPatch) ) ) ;

}








