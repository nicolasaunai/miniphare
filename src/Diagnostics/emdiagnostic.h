#ifndef EMDIAGNOSTIC_H
#define EMDIAGNOSTIC_H

#include <memory>

#include "AMR/patch.h"
#include "Diagnostics/fielddiagnostic.h"


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
    FieldPack virtual compute(Patch const& patch) override
    {

    }
};


class EMDiagnosticFactory
{
public:
    static std::unique_ptr<EMDiagnostic> createEMDiagnostic(uint32 id, std::string type)
    {
        if (type=="E")
        {
            std::unique_ptr<FieldDiagnosticComputeStrategy> strat{new ElectricDiag{}};
            std::unique_ptr<EMDiagnostic> ptr{new EMDiagnostic{id, std::move(strat)}};
        }
    }
};





#endif // EMDIAGNOSTIC_H
