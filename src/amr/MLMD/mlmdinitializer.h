#ifndef MLMDINITIALIZER_H
#define MLMDINITIALIZER_H

#include <vector>


#include "amr/MLMD/mlmdinfo.h"
#include "amr/Patch/patchinfo.h"
#include "data/grid/gridlayout.h"
#include "utilities/types.h"



/**
 * @brief The MLMDInitializer struct contains
 * MLMD parameters
 *
 *
 */
class MLMDInitializer
{
public:
    GridLayout baseLayout;
    PatchInfos patchInfos;

    MLMDInfos mlmdInfos;

public:
    MLMDInitializer(GridLayout const& baseLayout, PatchInfos const& patchInfos,
                    MLMDInfos const& mlmdInfos)
        : baseLayout{baseLayout}
        , patchInfos{patchInfos}
        , mlmdInfos{mlmdInfos}
    {
    }

    MLMDInitializer() = default;
};



#endif // MLMDINITIALIZER_H
