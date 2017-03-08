
#include <cmath>

#include "mlmd.h"
#include "hierarchy.h"
#include "patch.h"
#include "patchdata.h"
#include "Solver/solver.h"



MLMD::MLMD(std::unique_ptr<InitializerFactory> initFactory)
    : baseLayout_{ GridLayout{initFactory->gridLayout()} },
      patchHierarchy_{ std::make_shared<Patch>(
                           initFactory->getRefinedBox(), baseLayout_,
                           PatchData{std::move(initFactory)}  ) }

{
    // will probably have to change the way objects are initialized.
    // if we want, at some point, start from an already existing hierarchy
    // (in case of restart for e.g.
}



void MLMD::initializeRootLevel()
{
    std::cout << "building root level...";
    Patch& rootLevel = patchHierarchy_.root();
    rootLevel.init();
    std::cout << " OK" << std::endl;

}


void MLMD::evolveFullDomain()
{

    // evolve fields and particle for a time step
    patchHierarchy_.evolveHierarchy() ;

#if 1
    // Here, AMR patches will say whether they need refinement
    // the ouput of this method is used by updateHierarchy()
    // Note for later: will probably not be called every time step.
    std::vector< std::vector<RefinementInfo> > refinementTable
            = patchHierarchy_.evaluateHierarchy() ;

    std::vector< std::vector<GridLayout> >  refinedLayouts
            = buildLayouts( refinementTable ) ;

    // new patches are created here if necessary
    // it depends on evaluateHierarchy()
    patchHierarchy_.updateHierarchy( refinementTable, refinedLayouts ) ;
#endif

}

/**
 * @brief MLMD::buildLayouts is in charge of creating GridLayout
 * objects using RefinementInfo objects
 *
 * The GridLayout objects will be used by updateHierarchy(...) to
 * build the new patches and add them to the patch hierarchy
 *
 * @param infoVector
 * @return
 */
std::vector< std::vector<GridLayout> >
MLMD::buildLayouts( std::vector< std::vector<RefinementInfo> > const & refinementTable )
{
    GridLayout const & L0 = baseLayout_ ;   // short notation

    std::vector< std::vector<GridLayout> > newLayouts ;

    uint32 nbrLevels = static_cast<uint32>(refinementTable.size()) ;

    for( uint32 iLevel=0 ; iLevel<nbrLevels ; ++iLevel )
    {
        uint32 nbrPatches = static_cast<uint32>(refinementTable[iLevel].size()) ;

        std::vector<GridLayout> layoutVector ;

        for( uint32 iPatch=0 ; iPatch<nbrPatches ; ++iPatch )
        {
            RefinementInfo const & info = refinementTable[iLevel][iPatch];

            Box newBox = info.box ;
            uint32 level = info.level ;

            // TODO: return the adequate GridLayout given newBox information
            // new spatial step sizes
            double dx = L0.dx()/std::pow( refinementRatio_, level ) ;
            double dy = L0.dy()/std::pow( refinementRatio_, level ) ;
            double dz = L0.dz()/std::pow( refinementRatio_, level ) ;

            // cell numbers
            uint32 nbx = static_cast<uint32>( std::ceil( (newBox.x1 - newBox.x0)/dx ) ) ;
            uint32 nby = static_cast<uint32>( std::ceil( (newBox.y1 - newBox.y0)/dy ) ) ;
            uint32 nbz = static_cast<uint32>( std::ceil( (newBox.z1 - newBox.z0)/dz ) ) ;

            // we create the layout of a new patch
            // and store it
            layoutVector.push_back( GridLayout({{dx, dy, dz}}, {{nbx, nby, nbz}},
                                               L0.nbDimensions(), L0.layoutName(),
                                               Point{newBox.x0, newBox.y0, newBox.z0}, L0.order() ) ) ;
        }

        newLayouts.push_back( std::move(layoutVector) ) ;
    }

    return newLayouts ;
}



