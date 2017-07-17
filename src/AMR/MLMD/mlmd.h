#ifndef MLMD_H
#define MLMD_H


#include "grid/gridlayout.h"

#include "AMR/patchinfo.h"
#include "AMR/Hierarchy/hierarchy.h"

#include "Splitting/splittingstrategy.h"
#include "Initializer/initializerfactory.h"




/**
 * @brief The MLMD class deals with all MLMD operations.
 *
 * MLMD is the object executes MLDM operations:
 *     - dealing with PRA (splitting, inter patch communications, field interpolations etc.)
 *     - refinement / patch creation and insertion into the Hierarchy
 *     -  etc.
 */
class MLMD
{
private:
    // TODO: should be initialized by reading inputs parameters
    GridLayout baseLayout_ ;
    PatchInfo patchInfos_;

    void evolvePlasma_(Hierarchy& hierarchy, uint32 refineRatio);
    void recursivEvolve_(Patch& patch, uint32 ilevel, uint32 refineRatio, uint32 nbrSteps);

    void manageParticlesInGhostDomain_(Patch& patch);     // MLMD step 1
    void evolve_(Patch& patch, uint32 nbrSteps);          // MLMD step 2
    void sendCorrectedFieldsToChildrenPRA_(Patch& patch); // MLMD step 3
    void interpolateFieldBCInTime_(Patch& patch);         // MLMD step 4
    void updateFieldsWithRefinedSolutions_(Patch& patch); // MLMD step 5

public:
    MLMD(InitializerFactory const& initFactory);
    void initializeRootLevel(Hierarchy& patchHierarchy);
    void evolveFullDomain(Hierarchy& patchHierarchy, uint32 iter);

    // TODO void computedtFromCFL(); // calculate dt from CFL and dx_ // for MLMD version
};

#endif // MLMD_H
