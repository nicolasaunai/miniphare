#ifndef EMDIAGNOSTICFACTORY_H
#define EMDIAGNOSTICFACTORY_H

#include <memory>
#include <string>

#include "utilities/types.h"

#include "diagnostics/FieldDiagnostics/Electromag/emdiagnostic.h"


class EMDiagnosticFactory
{
public:
    static std::unique_ptr<EMDiagnostic> createEMDiagnostic(uint32 id, std::string diagName,
                                                            std::string path, std::string type)
    {
        if (type == "E")
        {
            std::unique_ptr<FieldDiagnosticComputeStrategy> strat{new ElectricDiag{}};
            std::unique_ptr<EMDiagnostic> ptr{
                new EMDiagnostic{id, diagName, path, std::move(strat)}};
            return ptr;
        }

        if (type == "B")
        {
            std::unique_ptr<FieldDiagnosticComputeStrategy> strat{new MagneticDiag{}};
            std::unique_ptr<EMDiagnostic> ptr{
                new EMDiagnostic{id, diagName, path, std::move(strat)}};
            return ptr;
        }

        return nullptr;
    }
};




#endif // EMDIAGNOSTICFACTORY_H
