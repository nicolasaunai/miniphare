#ifndef FIELDDIAGNOSTIC_H
#define FIELDDIAGNOSTIC_H

#include <vector>
#include <array>

#include "fieldpack.h"
#include "Field/field.h"
#include "utilityphare.h"
#include "grid/gridlayout.h"
#include "vecfield/vecfield.h"
#include "Diagnostics/diagnostics.h"
#include "fielddiagnosticcomputestrategy.h"








/**
 * @brief The FieldDiagnostic class is the interface used by the ExportStrategy
 * to get data that is going to be written on disk. It encapsulates a container
 * of FieldPack and an abstract strategy that will compute (or just get) a field
 * and put it in a FieldPack. The FieldDiagnostic::compute() method is in charge
 * of looping over the patch Hierarchy and give each of the Patches to a concrete
 * compute strategy.
 */
class FieldDiagnostic : public Diagnostic
{

protected:

    std::vector<FieldPack> packs_;  // one pack per patch
    std::unique_ptr<FieldDiagnosticComputeStrategy> strat_;


    FieldDiagnostic(uint32 id, std::string diagName, std::unique_ptr<FieldDiagnosticComputeStrategy> strat)
        : Diagnostic{id, diagName},
          strat_{std::move(strat)}
    {}


 public:

    // routines used to access the diagnostic data per patch by export strat.
    std::vector<FieldPack> const& getPacks() const {return packs_;}
    void flushPacks();
    std::string const& stratName() const {return strat_->name();}

    virtual void compute(Hierarchy const& hierarchy) final;


};



#endif // FIELDDIAGNOSTIC_H
