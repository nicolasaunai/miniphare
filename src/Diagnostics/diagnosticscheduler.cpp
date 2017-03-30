#include "diagnosticscheduler.h"



void DiagnosticScheduler::registerDiagnostic(DiagType type,
                        std::vector<uint32> const& computingIterations,
                        std::vector<uint32> const& writingIterations)
{
    computingIterations_.insert({type, computingIterations}  );
    writingIterations_.insert(  {type, writingIterations} );
}

