#include "fluiddiagnostic.h"


FieldPack RhoSpeciesDiag::compute(Patch const& patch)
{
    std::cout << "computing RhoSpeciesDiag" << speciesName_ << std::endl;
    FieldPack pack;

    PatchData const& patchData = patch.data();
    Field const& rho_s = patchData.ions().species(speciesName_).rho();
    GridLayout const& layout = patch.layout();

    fillPack_(pack, rho_s, layout);

    return pack;
}



FieldPack FluxSpeciesDiag::compute(Patch const& patch)
{
    std::cout << "computing FluxSpeciesDiag for species " << speciesName_ << std::endl;
    FieldPack pack;

    PatchData const& patchData = patch.data();
    Field const& fluxX_s = patchData.ions().species(speciesName_).flux(0);
    Field const& fluxY_s = patchData.ions().species(speciesName_).flux(1);
    Field const& fluxZ_s = patchData.ions().species(speciesName_).flux(2);
    GridLayout const& layout = patch.layout();

    fillPack_(pack, fluxX_s, layout);
    fillPack_(pack, fluxY_s, layout);
    fillPack_(pack, fluxZ_s, layout);

    return pack;
}
