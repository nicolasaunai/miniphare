

#include "diagnosticmanager.h"
#include "FieldDiagnostics/Electromag/emdiagnostic.h"
#include "FieldDiagnostics/Electromag/emdiagnosticfactory.h"
#include "FieldDiagnostics/Fluid/fluiddiagnostic.h"
#include "FieldDiagnostics/Fluid/fluiddiagnosticfactory.h"

uint32 DiagnosticsManager::id = 0;

DiagnosticsManager::DiagnosticsManager(std::unique_ptr<DiagnosticInitializer> initializer)
    : fluidDiags_{}
    , emDiags_{}
    , exportStrat_{ExportStrategyFactory::makeExportStrategy(initializer->exportType)}
    , scheduler_{}
{
    // first initialize all electromagnetic diagnostics
    for (uint32 iDiag = 0; iDiag < initializer->emInitializers.size(); ++iDiag)
    {
        newEMDiagnostic(initializer->emInitializers[iDiag].typeName,
                        initializer->emInitializers[iDiag].diagName,
                        initializer->emInitializers[iDiag].computingIterations,
                        initializer->emInitializers[iDiag].writingIterations);
    }

    // then initialize all Fluid diagnostics
    for (uint32 iDiag = 0; iDiag < initializer->fluidInitializers.size(); ++iDiag)
    {
        newFluidDiagnostic(initializer->fluidInitializers[iDiag].typeName,
                           initializer->emInitializers[iDiag].diagName,
                           initializer->fluidInitializers[iDiag].speciesName,
                           initializer->fluidInitializers[iDiag].computingIterations,
                           initializer->fluidInitializers[iDiag].writingIterations);
    }

    // then initialize all particle, orbit, probes, etc. diagnostics
}

// TODO : add 'id' to diagnostic fields too so that each of them
// know their id.
void DiagnosticsManager::newFluidDiagnostic(std::string type, std::string diagName,
                                            std::string speciesName,
                                            std::vector<uint32> const& computingIterations,
                                            std::vector<uint32> const& writingIterations)
{
    std::unique_ptr<FluidDiagnostic> fd
        = FluidDiagnosticFactory::createFluidDiagnostic(id, diagName, type, speciesName);
    fluidDiags_.push_back(std::move(fd));
    scheduler_.registerDiagnostic(id, computingIterations, writingIterations);
    id++; // new diagnostic identifier
}

void DiagnosticsManager::newEMDiagnostic(std::string type, std::string diagName,
                                         std::vector<uint32> const& computingIterations,
                                         std::vector<uint32> const& writingIterations)
{
    std::unique_ptr<EMDiagnostic> emd = EMDiagnosticFactory::createEMDiagnostic(id, diagName, type);
    emDiags_.push_back(std::move(emd));
    scheduler_.registerDiagnostic(id, computingIterations, writingIterations);
    id++; // new diagnostic identifier
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
        if (scheduler_.isTimeToCompute(timeManager, diag->id()))
            diag->compute(hierarchy);
    }

    for (auto& diag : fluidDiags_)
    {
        if (scheduler_.isTimeToCompute(timeManager, diag->id()))
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
        {
            exportStrat_->saveEMDiagnostic(*diag, timeManager);
            diag->flushPacks();
        }
    }

    for (auto& diag : fluidDiags_)
    {
        if (scheduler_.isTimeToWrite(timeManager, diag->id()))
        {
            exportStrat_->saveFluidDiagnostic(*diag, timeManager);
            diag->flushPacks();
        }
    }
}
