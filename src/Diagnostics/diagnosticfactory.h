#ifndef DIAGNOSTICFACTORY_H
#define DIAGNOSTICFACTORY_H

#include<memory>

#include "Diagnostics/diagnostics.h"
#include "electromagdiagnostic.h"


class DiagnosticFactory
{
public:

    // for now no other option than the type.
    static std::unique_ptr<Diagnostic> makeDiagnostic(DiagType type)
    {
        if (type == DiagType::E)
        {
            return std::unique_ptr<Diagnostic> {new ElectricDiagnostic{} };
        }

        else if (type == DiagType::B)
        {
            return std::unique_ptr<Diagnostic> {new MagneticDiagnostic{} };
        }

        return nullptr;
    }
};




#endif // DIAGNOSTICFACTORY_H
