#ifndef DIAGNOSTICINITIALIZER_H
#define DIAGNOSTICINITIALIZER_H

#include <vector>

#include "types.h"
#include "Diagnostics/diagtype.h"
#include "Diagnostics/exportstrategytypes.h"


struct DiagnosticInitializer
{
    std::vector<DiagType> diagTypes;
    std::vector< std::vector<uint32> > computingIterations;
    std::vector<std::vector<uint32> > writingIterations;
    ExportStrategyType exportType;
};


#endif // DIAGNOSTICINITIALIZER_H
