#ifndef FLUIDDIAGNOSTIC_H
#define FLUIDDIAGNOSTIC_H

#include <vector>
#include <string>

#include "AMR/patch.h"
#include "Field/field.h"
#include "AMR/patchdata.h"
#include "Plasmas/species.h"
#include "Diagnostics/fielddiagnostic.h"




class FluidDiagnostic : public FieldDiagnostic
{
private:
    std::string speciesName_;

public:

    FluidDiagnostic(uint32 id, std::string speciesName)
        : FieldDiagnostic{id, "fluid_"+speciesName},
          speciesName_{speciesName} {}

    std::string const& speciesName() {return speciesName_;}
};



struct FieldPack2
{
    std::vector<float> data_;
    std::array<uint32, 3> nbrNodes_;
    std::array<QtyCentering,3> centerings_;
    uint32 nbrDimensions_;
};



class RhoSpeciesDiag : FieldDiagnosticComputeStrategy
{
private:
    uint32 speciesID_;

public:
    RhoSpeciesDiag(uint32 id) : speciesID_{id}{}

    FieldPack virtual compute(Patch const& patch) override
    {
        std::cout << "computing RhoSpeciesDiag for species " << speciesID_ << std::endl;
        FieldPack pack;

        PatchData const& patchData = patch.data();
        Field const& rho_s = patchData.ions().species(speciesID_).rho();
        GridLayout const& layout = patch.layout();

        fillPack_(pack, rho_s, layout);

        return pack;
    }
};




#endif // FLUIDDIAGNOSTIC_H
