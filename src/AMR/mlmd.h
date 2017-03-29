#ifndef MLMD_H
#define MLMD_H


#include "hierarchy.h"
#include "Initializer/initializerfactory.h"
#include "grid/gridlayout.h"
#include "Splitting/splittingstrategy.h"



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
    uint32 refinementRatio_ {2};   // temporary hard-coded refinement strategy
    GridLayout baseLayout_ ;
    Hierarchy patchHierarchy_;

    const std::vector<uint32>  interpolationOrders_ ;
    const std::string pusher_ ;

    // WARNING: we assume the same SplittingStrategy
    // for all species, this is wrong if we use a different
    // interpolation order for each species
    std::unique_ptr<SplittingStrategy> splitStrategy_ ;


public:

    MLMD(InitializerFactory const& initFactory);

    void initializeRootLevel();

    void evolveFullDomain();

//    Hierarchy & hierarchy() { return patchHierarchy_ ; }
//    Hierarchy const & hierarchy() const { return patchHierarchy_ ; }


   // TODO void computedtFromCFL(); // calculate dt from CFL and dx_ // for MLMD version


};

#endif // MLMD_H
