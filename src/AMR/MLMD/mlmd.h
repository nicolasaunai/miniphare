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


public:

    MLMD(InitializerFactory const& initFactory);
    void initializeRootLevel(Hierarchy& patchHierarchy);
    void evolveFullDomain(Hierarchy& patchHierarchy);

//    Hierarchy & hierarchy() { return patchHierarchy_ ; }
//    Hierarchy const & hierarchy() const { return patchHierarchy_ ; }


   // TODO void computedtFromCFL(); // calculate dt from CFL and dx_ // for MLMD version


};

#endif // MLMD_H
