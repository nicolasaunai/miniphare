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
 * @brief encapsulate all diagnostics (fields, particle, globals, etc.)
 */

class DiagnosticsManager
{
private:

    DiagUnorderedMap< DiagType, std::shared_ptr<Diagnostic> > diags_;
    std::unique_ptr<ExportStrategy> exportStrat_;
    DiagnosticScheduler scheduler_;

public:


    // hard coded for now in the initialization list
    // will have to have a add_diag() function at some point
    // this will come from the factory..
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
