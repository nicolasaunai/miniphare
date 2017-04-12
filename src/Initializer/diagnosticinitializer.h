#ifndef DIAGNOSTICINITIALIZER_H
#define DIAGNOSTICINITIALIZER_H

#include <vector>
#include <string>

#include "types.h"
#include "Diagnostics/diagtype.h"
#include "Diagnostics/Export/exportstrategytypes.h"



struct FluidDiagInitializer
{
    std::string speciesName;
    std::string typeName;
    std::vector<uint32> computingIterations;
    std::vector<uint32>  writingIterations;
};

struct EMDiagInitializer
{
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
