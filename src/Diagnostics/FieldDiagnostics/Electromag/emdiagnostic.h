#ifndef EMDIAGNOSTIC_H
#define EMDIAGNOSTIC_H

#include <memory>

#include "AMR/patch.h"
#include "Electromag/electromag.h"
#include "Diagnostics/FieldDiagnostics/fielddiagnostic.h"


class EMDiagnostic : public FieldDiagnostic
{
private:

public:
    EMDiagnostic(uint32 id,std::unique_ptr<FieldDiagnosticComputeStrategy> strat)
        : FieldDiagnostic{id, "EMDiag", std::move(strat)}{}

};



class ElectricDiag : public FieldDiagnosticComputeStrategy
{
public:
    ElectricDiag() : FieldDiagnosticComputeStrategy{"E"} {}

    FieldPack virtual compute(Patch const& patch) override
    {
        std::cout << "computing ElectricDiag " << std::endl;
        FieldPack pack;

        PatchData const& patchData = patch.data();
        Electromag const& EMFields = patchData.EMfields();
        GridLayout const& layout = patch.layout();
        Field const& Ex = EMFields.getEi(0);
        Field const& Ey = EMFields.getEi(1);
        Field const& Ez = EMFields.getEi(2);

        fillPack_(pack, Ex, layout);
        fillPack_(pack, Ey, layout);
        fillPack_(pack, Ez, layout);

        return pack;
    }
};




#endif // EMDIAGNOSTIC_H
