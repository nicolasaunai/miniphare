#ifndef MLMD_H
#define MLMD_H


#include "data/grid/gridlayout.h"

#include "amr/Hierarchy/hierarchy.h"
#include "amr/Patch/patchinfo.h"
#include "amr/Splitting/splittingstrategy.h"

#include "initializer/initializerfactory.h"


#include "amr/Patch/patchboundarycondition.h"


/**
 * @brief The MLMD class deals with all MLMD operations.
 *
 * MLMD is the object executes MLDM operations:
 *     - dealing with GCA (splitting, inter patch communications, field interpolations etc.)
 *     - refinement / patch creation and insertion into the Hierarchy
 *     -  etc.
 */
class MLMD
{
private:
    // TODO: should be initialized by reading inputs parameters
    GridLayout baseLayout_;
    PatchInfo patchInfos_;

    void evolvePlasma_(Hierarchy& hierarchy, uint32 refineRatio);
    void recursivEvolve_(Patch& patch, uint32 ilevel, uint32 refineRatio, uint32 nbrSteps);

    void initGCAparticlesAndMoments_(Patch& patch);                                 // MLMD step 1
    void evolve_(Patch& patch, uint32 nbrSteps);                                    // MLMD step 2
    void sendCorrectedFieldsToChildrenGCA_(Patch const& parentPatch, Patch& patch); // MLMD step 3
    void updateFieldsWithRefinedSolutions_(Patch& parentPatch);                     // MLMD step 5

    void initGCAparticles_(BoundaryCondition* boundaryCondition);

    void computeGCADensityAndFlux_(BoundaryCondition* boundaryCondition, uint32 order);
    void computeGCAChargeDensity_(BoundaryCondition* boundaryCondition);

    void updateGCA_EMfields_(Patch& childPatch);

    void resetFreeEvolutionOfChildren_(Patch& parentPatch);

    void resetFreeEvolutionTime_(Patch& childPatch);
    void updateFreeEvolutionTime_(Patch& patch);

    void addChildVecFieldToPatch(VecField& parentVf, VecField const& childVf,
                                 GridLayout const& parentLayout, GridLayout const& childLayout);

    void addChildFieldToPatch1D_(Field& parentField, Field const& childField,
                                 GridLayout const& parentLayout, GridLayout const& childLayout);
    void addChildFieldToPatch2D_(Field& parentField, Field const& childField,
                                 GridLayout const& parentLayout, GridLayout const& childLayout);
    void addChildFieldToPatch3D_(Field& parentField, Field const& childField,
                                 GridLayout const& parentLayout, GridLayout const& childLayout);

    std::array<uint32, 3> getStartIndexes_(GridLayout const& childLayout, Field const& childField);
    std::array<uint32, 3> getEndIndexes_(GridLayout const& childLayout, Field const& childField);


public:
    MLMD(InitializerFactory const& initFactory);
    void initializeRootLevel(Hierarchy& patchHierarchy);
    void evolveFullDomain(Hierarchy& patchHierarchy, uint32 iter);

    // TODO void computedtFromCFL(); // calculate dt from CFL and dx_ // for MLMD version
};

#endif // MLMD_H
