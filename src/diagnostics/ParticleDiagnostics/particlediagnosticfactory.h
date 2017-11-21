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
        std::unique_ptr<ParticleDiagnosticStrategy> strat_ptr = nullptr;
        std::unique_ptr<ParticleDiagnostic> diag_ptr          = nullptr;

        if (selectorType == "spaceBox")
        {
            Box selectionBox{selectorParams[0], selectorParams[1]};

            std::unique_ptr<ParticleSelector> selector{new IsInBoxSelector{selectionBox}};

            strat_ptr = std::unique_ptr<ParticleDiagnosticStrategy>{
                new ParticleDiagnosticStrategy{selectorType, speciesName, std::move(selector)}};
        }

        if (selectorType == "spaceAndEnergyBox")
        {
            Box selectionBox{selectorParams[0], selectorParams[1]};

            // TODO: create space and energy selector
            std::unique_ptr<ParticleSelector> selector{new IsInBoxSelector{selectionBox}};

            strat_ptr = std::unique_ptr<ParticleDiagnosticStrategy>{
                new ParticleDiagnosticStrategy{selectorType, speciesName, std::move(selector)}};
        }

        diag_ptr = std::unique_ptr<ParticleDiagnostic>{new ParticleDiagnostic{
            id, type + "_" + selectorType, speciesName, std::move(strat_ptr)}};

        return diag_ptr;
    }
};


#endif // PARTDIAGNOSTICFACTORY_H
