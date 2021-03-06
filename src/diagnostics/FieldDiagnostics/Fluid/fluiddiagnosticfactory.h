#ifndef FLUIDDIAGNOSTICFACTORY_H
#define FLUIDDIAGNOSTICFACTORY_H


#include <memory>
#include <string>

#include "diagnostics/FieldDiagnostics/Fluid/fluiddiagnostic.h"
#include "diagnostics/FieldDiagnostics/fielddiagnosticcomputestrategy.h"



class FluidDiagnosticFactory
{
public:
    static std::unique_ptr<FluidDiagnostic> createFluidDiagnostic(uint32 id, std::string diagName,
                                                                  std::string path,
                                                                  std::string type,
                                                                  std::string speciesName)
    {
        if (type == "rho_s")
        {
            std::unique_ptr<FieldDiagnosticComputeStrategy> strat{new RhoSpeciesDiag{speciesName}};

            std::unique_ptr<FluidDiagnostic> ptr{
                new FluidDiagnostic{id, diagName, path, speciesName, std::move(strat)}};

            return ptr;
        }

        if (type == "flux_s")
        {
            std::unique_ptr<FieldDiagnosticComputeStrategy> strat{new FluxSpeciesDiag{speciesName}};

            std::unique_ptr<FluidDiagnostic> ptr{
                new FluidDiagnostic{id, diagName, path, speciesName, std::move(strat)}};

            return ptr;
        }


        return nullptr;
    }
};


#endif // FLUIDDIAGNOSTICFACTORY_H
