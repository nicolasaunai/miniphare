
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
Hierarchy::evaluateHierarchy( uint32 refineRatio,
                              GridLayout const & baseLayout )
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

                struct RefinementInfo refine{ patch, refineBox,
                            iLevel+1, refineRatio, baseLayout } ;

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
        std::vector< std::vector<RefinementInfo> > const & refinementTable )
{

    uint32 nbrLevels = static_cast<uint32>(refinementTable.size()) ;

    for( uint32 iLevel=0 ; iLevel<nbrLevels ; ++iLevel )
    {
        uint32 nbrPatches = static_cast<uint32>( refinementTable[iLevel].size() ) ;

        for( uint32 iPatch=0 ; iPatch<nbrPatches ; ++iPatch )
        {
            // GridLayout const & layout = layoutTable[iLevel][iPatch] ;
            RefinementInfo const & info = refinementTable[iLevel][iPatch] ;

            // create new Patch and update Hierarchy
            addNewPatch( info ) ;

            // TODO: call patch.init to initialize patch content


        }
    }

}



void Hierarchy::addNewPatch( RefinementInfo const & info)
{

    std::shared_ptr<Patch> coarsePatch = info.parentPatch ;
    Box refinedBox = info.box ;
    uint32 refinedLevel = info.level ;

    GridLayout refinedLayout = buildLayout( info ) ;

    // we need to build a factory for PatchData to be built
    std::unique_ptr<InitializerFactory>
            factory { new MLMDInitializerFactory(coarsePatch, refinedBox, refinedLayout) } ;

    Patch theNewPatch{ refinedBox, refinedLayout, PatchData{*factory} };

    std::shared_ptr<Patch> patchPtr = std::make_shared<Patch>( std::move(theNewPatch) ) ;

    // somehow attach this new patch to the hierarchy...
    coarsePatch->updateChildren( patchPtr ) ;

    // update the hierarchy
    patchTable_[refinedLevel].push_back( patchPtr ) ;

}

/**
 * @brief Hierarchy::buildLayout is in charge of creating a GridLayout
 * using a RefinementInfo object
 *
 * This GridLayout will be directly used by addNewPatch(...) to
 * build the new patch
 *
 * @param info
 * @return
 */
GridLayout  Hierarchy::buildLayout( RefinementInfo const & info )
{
    Box newBox = info.box ;
    uint32 level = info.level ;

    uint32 refineRatio = info.refinementRatio ;
    GridLayout const & L0 = info.baseLayout ;

    // TODO: return the adequate GridLayout given newBox information
    // new spatial step sizes
    double dx = L0.dx()/std::pow( refineRatio, level ) ;
    double dy = L0.dy()/std::pow( refineRatio, level ) ;
    double dz = L0.dz()/std::pow( refineRatio, level ) ;

    // cell numbers
    uint32 nbx = static_cast<uint32>( std::ceil( (newBox.x1 - newBox.x0)/dx ) ) ;
    uint32 nby = static_cast<uint32>( std::ceil( (newBox.y1 - newBox.y0)/dy ) ) ;
    uint32 nbz = static_cast<uint32>( std::ceil( (newBox.z1 - newBox.z0)/dz ) ) ;

    // we create the layout of a new patch
    // and store it
    return GridLayout({{dx, dy, dz}}, {{nbx, nby, nbz}},
                      L0.nbDimensions(), L0.layoutName(),
                      Point{newBox.x0, newBox.y0, newBox.z0}, L0.order() ) ;
}





