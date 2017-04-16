
#include <cmath>

#include "Solver/solver.h"

#include "AMR/patch.h"
#include "AMR/patchdata.h"
#include "AMR/MLMD/mlmd.h"
#include "AMR/Hierarchy/hierarchy.h"




MLMD::MLMD(InitializerFactory const& initFactory)
    : baseLayout_{ GridLayout{initFactory.gridLayout()} }
      /*patchHierarchy_{ std::make_shared<Patch>(
                           initFactory.getBox(), baseLayout_,
                           PatchData{initFactory}  ) },*/
{
    patchInfos_.interpOrders = initFactory.interpolationOrders();
    patchInfos_.pusher = initFactory.pusher();
    patchInfos_.splitStrategies = initFactory.splittingStrategies();
    patchInfos_.refinementRatio = 2;
    // will probably have to change the way objects are initialized.
    // if we want, at some point, start from an already existing hierarchy
    // (in case of restart for e.g.
}




void MLMD::initializeRootLevel(Hierarchy& patchHierarchy)
{
    std::cout << "building root level...";
    Patch& rootLevel = patchHierarchy.root();
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
void MLMD::evolveFullDomain(Hierarchy& patchHierarchy)
{

    // evolve fields and particle for a time step
    patchHierarchy.evolveDomainForOneTimeStep() ;

    // Here, AMR patches will say whether they need refinement
    // the ouput of this method is used by updateHierarchy()
    // Note for later: will probably not be called every time step.
    std::vector< std::vector<RefinementInfo> > refinementTable
            = patchHierarchy.evaluateRefinementNeed(patchInfos_.refinementRatio, baseLayout_ ) ;

    // New patches are created here if necessary
    // it depends on evaluateHierarchy()
    patchHierarchy.refine(refinementTable, patchInfos_);

}




