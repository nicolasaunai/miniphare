
#include <cmath>

#include "mlmd.h"
#include "hierarchy.h"
#include "patch.h"
#include "patchdata.h"
#include "Solver/solver.h"




MLMD::MLMD(InitializerFactory const& initFactory)
    : baseLayout_{ GridLayout{initFactory.gridLayout()} },
      patchHierarchy_{ std::make_shared<Patch>(
                           initFactory.getBox(), baseLayout_,
                           PatchData{initFactory}  ) },
      interpolationOrders_{  initFactory.interpolationOrders() },
      pusher_{ initFactory.pusher() },
      splitStrategy_{ initFactory.createSplittingStrategy() }
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


/**
 * @brief MLMD::evolveFullDomain contains the main operations
 * achieved in one computational cycle
 * For the moment:
 * - we evolve fields and particles
 * - we evaluate the consistency of the refinement with the physical processes
 * - if necessary the hierarchy is updated with new patches
 * - ...
 *
 * Diagnostics will be added soon
 *
 *
 */
void MLMD::evolveFullDomain()
{

    // evolve fields and particle for a time step
    patchHierarchy_.evolveDomainForOneTimeStep() ;

#if 1
    // Here, AMR patches will say whether they need refinement
    // the ouput of this method is used by updateHierarchy()
    // Note for later: will probably not be called every time step.
    std::vector< std::vector<RefinementInfo> > refinementTable
            = patchHierarchy_.evaluateRefinementNeed( refinementRatio_, baseLayout_ ) ;

    // New patches are created here if necessary
    // it depends on evaluateHierarchy()
    patchHierarchy_.updateHierarchy( refinementTable,
                                     refinementRatio_,
                                     interpolationOrders_, pusher_,
                                     *splitStrategy_ ) ;
#endif

}




