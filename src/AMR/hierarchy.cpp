
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
void Hierarchy::evaluateHierarchy()
{

    uint32 nbrLevels = static_cast<uint32>(patchTable_.size()) ;

    for( uint32 iLevel=0 ; iLevel<nbrLevels ; iLevel++ )
    {
        auto & patchesAtLevel = patchTable_[iLevel] ;

        for( std::shared_ptr<Patch> patch: patchesAtLevel)
        {
            RefinementAnalyser analyser{} ;

            // if the patch has to be refined we store a reference
            // for further use
            if( patch->checkRefinment( analyser ) )
            {
                Box refineBox = analyser.refinedArea() ;

                struct RefinementInfo refine{ patch, refineBox, iLevel+1 } ;

                patchToBeRefined_.push_back( refine ) ;
            }
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
void Hierarchy::updateHierarchy( GridLayout const & layoutL0, uint32 const & refinement )
{

    // Go through the structure, to look where refinement
    // is needed
    for( RefinementInfo const & info: patchToBeRefined_ )
    {
        // Whenever refinement is needed,
        // trigger refinement
        addNewPatch( info, layoutL0,
                     refinement ) ;

        // TODO: call patch.init


    }

    // patches to be refined have been treated
    patchToBeRefined_.clear() ;

}



void Hierarchy::addNewPatch( RefinementInfo const & info, GridLayout const & layoutL0,
                             uint32 const & refinement )
{

    std::shared_ptr<Patch> parent = info.parentPatch ;
    Box newBox = info.box ;

    // Build new GridLayout
    GridLayout newLayout{ buildNewLayout(info, layoutL0, refinement) } ;

    // we need to build a factory for PatchData to be built
    std::unique_ptr<InitializerFactory>
            factory { new MLMDInitializerFactory(parent, newBox, newLayout) } ;

    // create the new patch, give it a PatchData to which we pass the factory
    Patch theNewPatch{ newBox, PatchData{ std::move(factory) } };

    // somehow attach this new patch to the hierarchy...
    parent->updateChildren( std::make_shared<Patch>( std::move(theNewPatch) ) ) ;

}


GridLayout  buildNewLayout( RefinementInfo const & info, GridLayout const & layoutL0,
                            uint32 refinement )
{
    Box newBox = info.box ;
    uint32 level = info.level ;

    // TODO: return the adequate GridLayout given newBox information




    //    GridLayout newLayout( {{dx, dy, dz}}, {{nbrCellx, nbrCelly, nbrCellz}},
    //                          nbDims_, layoutName_,
    //                          origin_, interpOrder_ );

    // fake GridLayout identical to L0 base layout
    return GridLayout( layoutL0.dxdydz(), layoutL0.nbrCellxyz(),
                       layoutL0.nbDimensions(), layoutL0.layoutName(),
                       layoutL0.origin(), layoutL0.order() ) ;
}






