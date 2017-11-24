#ifndef PARTDIAGNOSTICFACTORY_H
#define PARTDIAGNOSTICFACTORY_H

#include <memory>
#include <string>

#include "diagnostics/ParticleDiagnostics/particlediagnostic.h"

#include "utilities/particleselector.h"


class ParticleDiagnosticFactory
{
public:
    static std::unique_ptr<ParticleDiagnostic>
    createParticleDiagnostic(uint32 id, std::string type, std::string selectorType,
                             std::vector<double> const& selectorParams, std::string speciesName)
    {
        std::unique_ptr<ParticleDiagnostic> diag_ptr = nullptr;
        std::unique_ptr<ParticleSelector> selector   = nullptr;

        if (selectorType == "spaceBox")
        {
            Box selectionBox{selectorParams[0], selectorParams[1]};

            selector = std::unique_ptr<ParticleSelector>{new IsInBoxSelector{selectionBox}};
        }

        if (selectorType == "spaceAndEnergyBox")
        {
            Box selectionBox{selectorParams[0], selectorParams[1]};

            // TODO: create space and energy selector
            selector = std::unique_ptr<ParticleSelector>{new IsInBoxSelector{selectionBox}};
        }

        diag_ptr = std::unique_ptr<ParticleDiagnostic>{new ParticleDiagnostic{
            id, type + "_" + selectorType, speciesName, selectorType, std::move(selector)}};

        return diag_ptr;
    }
};


#endif // PARTDIAGNOSTICFACTORY_H
