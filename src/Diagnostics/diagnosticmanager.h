#ifndef DIAGNOSTICMANAGER_H
#define DIAGNOSTICMANAGER_H


#include <memory>
#include <vector>

#include "AMR/patchdata.h"
#include "grid/gridlayout.h"

#include "Diagnostics/diagtype.h"
#include "diagnosticscheduler.h"

#include "Initializer/diagnosticinitializer.h"

#include "Diagnostics/Export/exportstrategy.h"
#include "Diagnostics/Export/exportstrategyfactory.h"

#include "Diagnostics/FieldDiagnostics/Electromag/emdiagnostic.h"
#include "Diagnostics/FieldDiagnostics/Fluid/fluiddiagnosticfactory.h"


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
    static uint32 id;
    std::vector<std::unique_ptr<FluidDiagnostic>> fluidDiags_;
    std::vector<std::unique_ptr<EMDiagnostic>> emDiags_;
    // std::vector<ParticleDiagnostic> partDiags_;
    // std::vector<OrbitDiagnostic> orbitDiags_;
    // std::vector<ProbeDiagnostic> probeDiags_;
    // std::vector<GlobalDiagnostic> globalDiags_;
    std::unique_ptr<ExportStrategy> exportStrat_;
    DiagnosticScheduler scheduler_;

public:
    /** @brief DiagnosticsManager creates an empty DiagnosticManager with concrete ExportStrategy */
    DiagnosticsManager(std::unique_ptr<DiagnosticInitializer> initializer);


    void newFluidDiagnostic(std::string type, std::string speciesName,
                            std::vector<uint32> const& computingIterations,
                            std::vector<uint32> const& writingIterations);

    void newEMDiagnostic(std::string type, std::vector<uint32> const& computingIterations,
                         std::vector<uint32> const& writingIterations);



    void compute(Time const& timeManager, Hierarchy const& hierarchy);

    void save(Time const& timeManager);

    ~DiagnosticsManager() = default;
};



#endif // DIAGNOSTICMANAGER_H
