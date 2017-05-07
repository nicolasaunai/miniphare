#include "diagnosticscheduler.h"




void DiagnosticScheduler::registerDiagnostic(uint32 id,
                                             std::vector<uint32> const& computingIterations,
                                             std::vector<uint32> const& writingIterations)
{
    computingIterations_.insert({id, computingIterations});
    writingIterations_.insert({id, writingIterations});
    nextComputingIterationIndex_.insert({id, 0});
    nextWritingIterationIndex_.insert({id, 0});
}
