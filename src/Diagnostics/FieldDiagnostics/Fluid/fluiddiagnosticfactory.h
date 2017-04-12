#ifndef FLUIDDIAGNOSTICFACTORY_H
#define FLUIDDIAGNOSTICFACTORY_H


#include <memory>
#include <string>

#include "types.h"

#include "Diagnostics/FieldDiagnostics/Fluid/fluiddiagnostic.h"
#include "Diagnostics/FieldDiagnostics/fielddiagnosticcomputestrategy.h"



class FluidDiagnosticFactory
{
public:
    static std::unique_ptr<FluidDiagnostic> createFluidDiagnostic(uint32 id, std::string type,
                                                                  std::string speciesName)
    {

        if (type == "rho_s")
        {
            std::unique_ptr<FieldDiagnosticComputeStrategy> strat{new RhoSpeciesDiag{speciesName}};

            std::unique_ptr<FluidDiagnostic> ptr{new FluidDiagnostic{id ,speciesName,
                                                                     std::move(strat) } };

            return ptr;
        }

        if (type == "flux_s")
        {
            std::unique_ptr<FieldDiagnosticComputeStrategy> strat{new FluxSpeciesDiag{speciesName}};

            std::unique_ptr<FluidDiagnostic> ptr{new FluidDiagnostic{id ,speciesName,
                                                                     std::move(strat) } };

            return ptr;
        }


        return nullptr;
    }
};


#endif // FLUIDDIAGNOSTICFACTORY_H
