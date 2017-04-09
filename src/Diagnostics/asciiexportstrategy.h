#ifndef ASCIIEXPORTSTRATEGY_H
#define ASCIIEXPORTSTRATEGY_H

#include "exportstrategy.h"
#include "Diagnostics/diagnostics.h"

// a concrete ExportStrategy is to write Ascii files
// here we will write one file per time step and per diagnostic
// this is the simplest and dumbest we can do
// any smarter ascii format will just be a pain in the ass to read
// this is mainly for 1D debug stuff... not for real usage
class AsciiExportStrategy : public ExportStrategy
{
public:

    virtual void saveEMDiagnostic(EMDiagnostic const& diag, Time const& timeManager) final;
    virtual void saveFluidDiagnostic(FluidDiagnostic const& diag, Time const& timeManager) final;

    virtual ~AsciiExportStrategy() = default;
};



#endif // ASCIIEXPORTSTRATEGY_H
