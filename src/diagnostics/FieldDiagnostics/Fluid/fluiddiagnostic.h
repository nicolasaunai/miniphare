#ifndef FLUIDDIAGNOSTIC_H
#define FLUIDDIAGNOSTIC_H

#include <string>
#include <vector>

#include "amr/Patch/patch.h"
#include "data/Field/field.h"
//#include "AMR/patchdata.h"
#include "diagnostics/FieldDiagnostics/fielddiagnostic.h"
//#include "Plasmas/species.h"




class FluidDiagnostic : public FieldDiagnostic
{
private:
    std::string speciesName_;

public:
    FluidDiagnostic(uint32 id, std::string diagname, std::string path, std::string speciesName,
                    std::unique_ptr<FieldDiagnosticComputeStrategy> strat)
        : FieldDiagnostic{id, diagname + "_" + "fluid_" + strat->name() + "_" + speciesName, path,
                          std::move(strat)}
        , speciesName_{speciesName}
    {
    }

    std::string const& speciesName() const { return speciesName_; }
};




class RhoSpeciesDiag : public FieldDiagnosticComputeStrategy
{
private:
    std::string speciesName_;

public:
    RhoSpeciesDiag(std::string speciesName)
        : FieldDiagnosticComputeStrategy{"rho_s"}
        , speciesName_{speciesName}
    {
    }

    FieldPack virtual compute(Patch const& patch) override;
};




class FluxSpeciesDiag : public FieldDiagnosticComputeStrategy
{
private:
    std::string speciesName_;

public:
    FluxSpeciesDiag(std::string speciesName)
        : FieldDiagnosticComputeStrategy{"flux_s"}
        , speciesName_{speciesName}
    {
    }

    FieldPack virtual compute(Patch const& patch) override;
};




#endif // FLUIDDIAGNOSTIC_H
