#ifndef EXPORTSTRATEGY_H
#define EXPORTSTRATEGY_H


#include "Time/time.h"
#include "Diagnostics/diagnostics.h"




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
    virtual void save(Diagnostic const& diag, Time const& timeManager) = 0;

};





#endif // EXPORTSTRATEGY_H
