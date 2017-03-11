
#include "mlmd.h"
#include "hierarchy.h"
#include "patch.h"
#include "patchdata.h"
#include "Solver/solver.h"



MLMD::MLMD(std::unique_ptr<InitializerFactory> initFactory)
    : baseLayout_{ GridLayout{initFactory->gridLayout()} }

{
    // will probably have to change the way objects are initialized.
    // if we want, at some point, start from an already existing hierarchy
    // (in case of restart for e.g.
    patchHierarchy_.setRootPatch( std::make_shared<Patch>( initFactory->getBox(),PatchData{std::move(initFactory)} ) );
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

        // fake GridLayout identical to L0 base layout
        // added for now
        newLayouts.push_back( GridLayout(L0.dxdydz(), L0.nbrCellxyz(),
                                         L0.nbDimensions(), L0.layoutName(),
                                         L0.origin(), L0.order() ) ) ;
    }

    return newLayouts ;
}



