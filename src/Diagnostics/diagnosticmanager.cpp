#include "diagnosticmanager.h"



DiagnosticsManager::DiagnosticsManager(std::unique_ptr<DiagnosticInitializer> initializer)
    : diags_{},
      exportStrat_{ExportStrategyFactory::makeExportStrategy(initializer->exportType)},
      scheduler_{}
{
    for (uint32 iDiag=0; iDiag < initializer->diagTypes.size(); ++iDiag)
    {
        newDiagnostic( initializer->diagTypes[iDiag],
                       initializer->computingIterations[iDiag],
                       initializer->writingIterations[iDiag]);
    }
}






/**
 * @brief DiagnosticsManager::newDiagnostic add a new kind of Diagnostic to the DiagnosticManager
 * @param type is the kind of Diagnostic to be added
 * @param computingIterations contains all iterations at which the Diagnostic
 * will have to be calculated
 * @param writingIterations contains all iterations at which the Diagnostic will be written to disk
 */
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




/**
 * @brief DiagnosticsManager::compute will calculate all diagnostics that need to be.
 * @param timeManager is used to know the current time/iteration
 * @param patchData owns the data used to get the Diagnotic calculated
 * @param layout has all information about the grid
 */
void DiagnosticsManager::compute(Time const& timeManager, Hierarchy const& hierarchy)
{
    for (auto const& diagPair : diags_)
    {
        DiagType type    =  diagPair.first;
        Diagnostic& diag = *diagPair.second;

        if (scheduler_.timeToCompute(timeManager, type) )
        {
            diag.compute(hierarchy);
        }
    }
}


/**
 * @brief DiagnosticsManager::save will save Diagnostics to the disk
 *
 * The function loop over all Diagnostic, ask the scheduler if it is time to
 * save data to disk and calls the ExportStrategy if yes.
 *
 * @param timeManager is used to get the current time and iteration
 */
void DiagnosticsManager::save(Time const& timeManager)
{
    for (auto const& diagPair : diags_)
    {
        DiagType type = diagPair.first;
        if (scheduler_.timeToWrite(timeManager, type) )
        {
            exportStrat_->save(*diagPair.second, timeManager);
        }
    }
}
