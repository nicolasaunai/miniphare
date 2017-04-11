#ifndef EMDIAGNOSTICFACTORY_H
#define EMDIAGNOSTICFACTORY_H

#include <memory>
#include <string>

#include "types.h"

#include "Diagnostics/FieldDiagnostics/Electromag/emdiagnostic.h"


class EMDiagnosticFactory
{
public:
    static std::unique_ptr<EMDiagnostic> createEMDiagnostic(uint32 id, std::string type)
    {
        if (type=="E")
        {
            std::unique_ptr<FieldDiagnosticComputeStrategy> strat{new ElectricDiag{}};
            std::unique_ptr<EMDiagnostic> ptr{new EMDiagnostic{id, std::move(strat)}};
        }
    }
};




#endif // EMDIAGNOSTICFACTORY_H
