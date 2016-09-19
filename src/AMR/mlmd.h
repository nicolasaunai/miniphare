#ifndef MLMD_H
#define MLMD_H


#include "hierarchy.h"
#include "Initializer/initializerfactory.h"
#include "grid/gridlayout.h"


/**
 * @brief The MLMD class deals with all MLMD operations.
 *
 * MLMD is the object executes MLDM operations:
 *     - dealing with PRA (splitting, inter patch communications, field interpolations etc.)
 *     - refinement / patch creation and insertion into the Hierarchy
 *     -  etc.
 */
class MLMD // TODO should be a singleton pattern
{

public:

    MLMD(std::unique_ptr<InitializerFactory> initFactory);

    void initializeRootLevel();


private:
    uint32 refinementRatio_ {2};
    Hierarchy patchHierarchy_;


    // PRAwidthx,
    // splitting

   // TODO GridLayout* generateLayoutFromRefineRatio() ....
   // TODO void computedtFromCFL(); // calculate dt from CFL and dx_ // for MLMD version


};

#endif // MLMD_H
