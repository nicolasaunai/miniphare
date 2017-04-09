#ifndef DIAGNOSTICINITIALIZER_H
#define DIAGNOSTICINITIALIZER_H

#include <vector>
#include <string>

#include "types.h"
#include "Diagnostics/diagtype.h"
#include "Diagnostics/exportstrategytypes.h"



struct FluidDiagInitializer
{
    std::string speciesName;
    std::vector< std::vector<uint32> > computingIterations;
    std::vector<std::vector<uint32> > writingIterations;
};

struct EMDiagInitializer
{
    std::vector< std::vector<uint32> > computingIterations;
    std::vector<std::vector<uint32> > writingIterations;
};




struct DiagnosticInitializer
{
    std::vector<EMDiagInitializer> emInitializers;
    std::vector<FluidDiagInitializer> fluidInitializers;
    // other kinds of diags
    ExportStrategyType exportType;
};


#endif // DIAGNOSTICINITIALIZER_H
