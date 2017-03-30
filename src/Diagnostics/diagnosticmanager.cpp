#include "diagnosticmanager.h"




void DiagnosticsManager::newDiagnostic(DiagType type,
                                       std::vector<uint32> const& computingIterations,
                                       std::vector<uint32> const& writingIterations)
{
    // create a new diagnostic of type 'type'
    std::shared_ptr<Diagnostic> newDiag = DiagnosticFactory::makeDiagnostic(type);
    diags_.insert( {type, newDiag} );

    // register to the scheduler
    scheduler_.registerDiagnostic(type, computingIterations, writingIterations);
}



void DiagnosticsManager::compute(Time const& timeManager,
                                 PatchData const& patchData, GridLayout const& layout)
{
    for (auto const& diagPair : diags_)
    {
        DiagType type    =  diagPair.first;
        Diagnostic& diag = *diagPair.second;

        if (scheduler_.timeToCompute(timeManager, type) )
        {
            diag.compute(patchData, layout);
        }
    }
}


void DiagnosticsManager::save(Time const& timeManager)
{
    for (auto const& diagPair : diags_)
    {
        exportStrat_->save(*diagPair.second, timeManager);
    }
}
