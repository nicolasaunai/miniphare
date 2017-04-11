#ifndef FLUIDDIAGNOSTIC_H
#define FLUIDDIAGNOSTIC_H

#include <vector>
#include <string>

#include "AMR/patch.h"
#include "Field/field.h"
#include "AMR/patchdata.h"
#include "Plasmas/species.h"
#include "Diagnostics/FieldDiagnostics/fielddiagnostic.h"




class FluidDiagnostic : public FieldDiagnostic
{
private:
    std::string speciesName_;

public:

    FluidDiagnostic(uint32 id,
                    std::string speciesName,
                    std::unique_ptr<FieldDiagnosticComputeStrategy> strat)
        : FieldDiagnostic{id, "fluid_"+speciesName, std::move(strat)},
          speciesName_{speciesName} {}

    std::string const& speciesName() const {return speciesName_;}
};




class RhoSpeciesDiag : public FieldDiagnosticComputeStrategy
{
private:
    std::string speciesName_;

public:
    RhoSpeciesDiag(std::string speciesName)
        : FieldDiagnosticComputeStrategy{"rho_s"},
          speciesName_{speciesName}{}

    FieldPack virtual compute(Patch const& patch) override
    {
        std::cout << "computing RhoSpeciesDiag for species " << speciesName_ << std::endl;
        FieldPack pack;

        PatchData const& patchData = patch.data();
        Field const& rho_s = patchData.ions().species(speciesName_).rho();
        GridLayout const& layout = patch.layout();

        fillPack_(pack, rho_s, layout);

        return pack;
    }
};




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
        return nullptr;
    }
};




#endif // FLUIDDIAGNOSTIC_H
