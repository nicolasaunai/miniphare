#ifndef DIAGNOSTICMANAGER_H
#define DIAGNOSTICMANAGER_H


#include <memory>
#include <vector>

#include "diagnosticscheduler.h"
#include "diagtype.h"

#include "diagnosticinitializer.h"

#include "Export/exportstrategy.h"
#include "Export/exportstrategyfactory.h"

#include "FieldDiagnostics/Electromag/emdiagnostic.h"
#include "FieldDiagnostics/Fluid/fluiddiagnostic.h"
#include "ParticleDiagnostics/particlediagnostic.h"


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
    std::vector<std::unique_ptr<ParticleDiagnostic>> partDiags_;

    // std::vector<OrbitDiagnostic> orbitDiags_;
    // std::vector<ProbeDiagnostic> probeDiags_;
    // std::vector<GlobalDiagnostic> globalDiags_;
    std::unique_ptr<ExportStrategy> exportStrat_;
    DiagnosticScheduler scheduler_;

public:
    /** @brief DiagnosticsManager creates an empty DiagnosticManager with concrete ExportStrategy */
    DiagnosticsManager(std::unique_ptr<DiagnosticInitializer> initializer);


    void newFluidDiagnostic(std::string type, std::string diagName, std::string path,
                            std::string speciesName, std::vector<uint32> const& computingIterations,
                            std::vector<uint32> const& writingIterations);

    void newEMDiagnostic(std::string type, std::string diagName, std::string path,
                         std::vector<uint32> const& computingIterations,
                         std::vector<uint32> const& writingIterations);

    void newParticleDiagnostic(PartDiagInitializer const& partInitializer);

    void compute(Time const& timeManager, Hierarchy const& hierarchy);

    void save(Time const& timeManager);

    ~DiagnosticsManager() = default;
};



#endif // DIAGNOSTICMANAGER_H
