#ifndef DIAGNOSTICINITIALIZER_H
#define DIAGNOSTICINITIALIZER_H

#include <string>
#include <vector>

#include "diagnostics/Export/exportstrategytypes.h"
#include "diagtype.h"
#include "utilities/types.h"



struct FluidDiagInitializer
{
    std::string diagName;
    std::string speciesName;
    std::string typeName;
    std::vector<uint32> computingIterations;
    std::vector<uint32> writingIterations;
};

struct EMDiagInitializer
{
    std::string diagName;
    std::string typeName;
    std::vector<uint32> computingIterations;
    std::vector<uint32> writingIterations;
};




struct DiagnosticInitializer
{
    std::vector<EMDiagInitializer> emInitializers;
    std::vector<FluidDiagInitializer> fluidInitializers;
    // other kinds of diags
    ExportStrategyType exportType;
};


#endif // DIAGNOSTICINITIALIZER_H
