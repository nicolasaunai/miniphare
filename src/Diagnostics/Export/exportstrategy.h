#ifndef EXPORTSTRATEGY_H
#define EXPORTSTRATEGY_H


#include "diagnostics/FieldDiagnostics/Electromag/emdiagnostic.h"
#include "diagnostics/FieldDiagnostics/Fluid/fluiddiagnostic.h"
#include "diagnostics/diagnostics.h"
#include "utilities/Time/pharetime.h"
//#include "Diagnostics/



// interface used to write data on disk
// it is used by the DiagnosticManager, which does
// not know which concrete strategy is used to write data on disk
// all concrete ExportStrategy will implement the save() method
// which will take, for a diagnostic 'diag' a ref to a standard DiagData
// structure and know how to write it in a concrete file format
// this is implemented as a bridge pattern
class ExportStrategy
{
private:

public:
    virtual void saveEMDiagnostic(EMDiagnostic const& diag, Time const& timeManager) = 0;
    virtual void saveFluidDiagnostic(FluidDiagnostic const& diag, Time const& timeManager) = 0;

    virtual ~ExportStrategy() = default;

};





#endif // EXPORTSTRATEGY_H
