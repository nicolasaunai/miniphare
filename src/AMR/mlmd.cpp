
#include <cmath>

#include "mlmd.h"
#include "hierarchy.h"
#include "patch.h"
#include "patchdata.h"
#include "Solver/solver.h"



MLMD::MLMD(std::unique_ptr<InitializerFactory> initFactory)
    : baseLayout_{ GridLayout{initFactory->gridLayout()} },
      patchHierarchy_{ std::make_shared<Patch>( initFactory->getBox(),PatchData{std::move(initFactory)}  ) }

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

#if 0
    // Here, AMR patches will say whether they need refinement
    // the ouput of this method is used by updateHierarchy()
    // Note for later: will probably not be called every time step.
    std::vector<RefinementInfo> patchesToBeCreated
            = patchHierarchy_.evaluateHierarchy() ;

    std::vector<GridLayout>  newLayouts
            = buildLayouts( patchesToBeCreated ) ;

    // new patches are created here if necessary
    // it depends on evaluateHierarchy()
    patchHierarchy_.updateHierarchy( newLayouts, patchesToBeCreated ) ;
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
std::vector<GridLayout>
MLMD::buildLayouts( std::vector<RefinementInfo> const & infoVector )
{
    std::vector<GridLayout>  newLayouts{} ;

    // short notation
    GridLayout const & L0 = baseLayout_ ;

    for( RefinementInfo const & info: infoVector )
    {
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
        newLayouts.push_back( GridLayout({{dx, dy, dz}}, {{nbx, nby, nbz}},
                                         L0.nbDimensions(), L0.layoutName(),
                                         Point{newBox.x0, newBox.y0, newBox.z0}, L0.order() ) ) ;
    }

    return newLayouts ;
}



