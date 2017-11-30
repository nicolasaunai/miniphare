#ifndef EMDIAGNOSTIC_H
#define EMDIAGNOSTIC_H

#include <memory>

#include "amr/Patch/patch.h"
#include "data/Electromag/electromag.h"
#include "diagnostics/FieldDiagnostics/fielddiagnostic.h"
#include <utilities/print/outputs.h>

class EMDiagnostic : public FieldDiagnostic
{
private:
public:
    EMDiagnostic(uint32 id, std::string diagName, std::string path,
                 std::unique_ptr<FieldDiagnosticComputeStrategy> strat)
        : FieldDiagnostic{id, diagName + "_" + strat->name(), path, std::move(strat)}
    {
    }
};



class ElectricDiag : public FieldDiagnosticComputeStrategy
{
public:
    ElectricDiag()
        : FieldDiagnosticComputeStrategy{"E"}
    {
    }

    FieldPack virtual compute(Patch const& patch) override
    {
        Logger::Debug << "\t - computing ElectricDiag \n";
        Logger::Debug.flush();
        FieldPack pack;

        PatchData const& patchData = patch.data();
        Electromag const& EMFields = patchData.EMfields();
        GridLayout const& layout   = patch.layout();
        Field const& Ex            = EMFields.getEi(0);
        Field const& Ey            = EMFields.getEi(1);
        Field const& Ez            = EMFields.getEi(2);

        fillPack_(pack, Ex, layout);
        fillPack_(pack, Ey, layout);
        fillPack_(pack, Ez, layout);

        return pack;
    }
};




class MagneticDiag : public FieldDiagnosticComputeStrategy
{
public:
    MagneticDiag()
        : FieldDiagnosticComputeStrategy{"B"}
    {
    }

    FieldPack virtual compute(Patch const& patch) override
    {
        Logger::Debug << "\t - computing MagneticDiag\n";
        Logger::Debug.flush();
        FieldPack pack;

        PatchData const& patchData = patch.data();
        Electromag const& EMFields = patchData.EMfields();
        GridLayout const& layout   = patch.layout();
        Field const& Bx            = EMFields.getBi(0);
        Field const& By            = EMFields.getBi(1);
        Field const& Bz            = EMFields.getBi(2);

        fillPack_(pack, Bx, layout);
        fillPack_(pack, By, layout);
        fillPack_(pack, Bz, layout);

        return pack;
    }
};




#endif // EMDIAGNOSTIC_H
