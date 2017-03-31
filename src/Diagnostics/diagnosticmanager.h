#ifndef DIAGNOSTICMANAGER_H
#define DIAGNOSTICMANAGER_H


#include <memory>
#include <vector>

#include "AMR/patchdata.h"
#include "exportstrategy.h"
#include "grid/gridlayout.h"
#include "Diagnostics/diagtype.h"
#include "diagnosticscheduler.h"
#include "diagnosticfactory.h"

/**
 * @brief The DiagnosticsManager class is the interface to manipulate all code diagnostics
 *
 * Field, particle, global, etc. diagnostics are all registered in the DiagnosticManager.
 * It also encapsulates:
 *  * a DiagnosticScheduler which knows wen to compute and write
 *  * an ExportStrategy which knows how to write Diagnostic on disks
 */
class DiagnosticsManager
{
private:

    DiagUnorderedMap< DiagType, std::shared_ptr<Diagnostic> > diags_;
    std::unique_ptr<ExportStrategy> exportStrat_;
    DiagnosticScheduler scheduler_;

public:

    /** @brief DiagnosticsManager creates an empty DiagnosticManager with concrete ExportStrategy */
    DiagnosticsManager(ExportStrategyType exportType)
        : diags_{},
          exportStrat_{ExportStrategyFactory::makeExportStrategy(exportType)},
          scheduler_{} {}

    void newDiagnostic(DiagType type,
                       std::vector<uint32> const& computingIterations,
                       std::vector<uint32> const& writingIterations);

    void compute(Time const& timeManager, PatchData const& patchData, GridLayout const& layout);

    void save(Time const& timeManager);

    ~DiagnosticsManager() = default;
};



#endif // DIAGNOSTICMANAGER_H
