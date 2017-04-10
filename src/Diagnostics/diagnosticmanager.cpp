#include "diagnosticmanager.h"
#include "Diagnostics/fluiddiagnostic.h"


uint32 DiagnosticsManager::id = 0;


DiagnosticsManager::DiagnosticsManager(std::unique_ptr<DiagnosticInitializer> initializer)
    : fluidDiags_{},
      emDiags_{},
      exportStrat_{ExportStrategyFactory::makeExportStrategy(initializer->exportType)},
      scheduler_{}
{
    for (uint32 iDiag=0; iDiag < initializer->emInitializers.size(); ++iDiag)
    {
        newEMDiagnostic(initializer->emInitializers[iDiag].typeName,
                        initializer->emInitializers[iDiag].computingIterations[iDiag],
                        initializer->emInitializers[iDiag].writingIterations[iDiag]);
    }

    for (uint32 iDiag=0; iDiag < initializer->fluidInitializers.size(); ++iDiag)
    {
        newFluidDiagnostic(initializer->fluidInitializers[iDiag].typeName,
                           initializer->fluidInitializers[iDiag].speciesName,
                           initializer->fluidInitializers[iDiag].computingIterations[iDiag],
                           initializer->fluidInitializers[iDiag].writingIterations[iDiag]);
    }
}






//TODO : add 'id' to diagnostic fields too so that each of them
// know their id.
void DiagnosticsManager::newFluidDiagnostic(std::string type, std::string speciesName,
                                           std::vector<uint32> const& computingIterations,
                                           std::vector<uint32> const& writingIterations)
{
    std::unique_ptr<FluidDiagnostic> fd = FluidDiagnosticFactory::createFluidDiagnostic(id, type, speciesName);
    fluidDiags_.push_back(std::move(fd));
    scheduler_.registerDiagnostic(id, computingIterations, writingIterations);
    id ++; // new diagnostic identifier
}



void DiagnosticsManager::newEMDiagnostic(std::string type,
                                         std::vector<uint32> const& computingIterations,
                                         std::vector<uint32> const& writingIterations)
{
    std::unique_ptr<EMDiagnostic> emd = EMDiagnosticFactory::createEMDiagnostic(id,type);
    emDiags_.push_back(std::move(emd));
    scheduler_.registerDiagnostic(id, computingIterations, writingIterations);
    id ++; // new diagnostic identifier
}






/**
 * @brief DiagnosticsManager::compute will calculate all diagnostics that need to be.
 * @param timeManager is used to know the current time/iteration
 * @param patchData owns the data used to get the Diagnotic calculated
 * @param layout has all information about the grid
 */
void DiagnosticsManager::compute(Time const& timeManager, Hierarchy const& hierarchy)
{
   for (auto& diag : emDiags_)
   {
       if ( scheduler_.isTimeToCompute(timeManager, diag->id()) )
            diag->compute(hierarchy);
   }

   for (auto& diag : fluidDiags_)
   {
       if ( scheduler_.isTimeToCompute(timeManager, diag->id()) )
            diag->compute(hierarchy);
   }

   // other kind of diagnostics here...

}


/**
 * @brief DiagnosticsManager::save will save Diagnostics to the disk
 *
 * The function loop over all Diagnostic, ask the scheduler if it is time to
 * save data to disk and calls the ExportStrategy if yes.
 * As soon as data is written, we get rid of it.
 *
 * @param timeManager is used to get the current time and iteration
 */
void DiagnosticsManager::save(Time const& timeManager)
{

    for (auto& diag : emDiags_)
    {
        if (scheduler_.isTimeToWrite(timeManager, diag->id()))
             exportStrat_->saveEMDiagnostic(*diag, timeManager);
    }


    for (auto& diag : fluidDiags_)
    {
        if (scheduler_.isTimeToWrite(timeManager, diag->id()))
             exportStrat_->saveFluidDiagnostic(*diag, timeManager);
    }
}
