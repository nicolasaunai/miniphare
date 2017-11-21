#ifndef PARTICLEDIAGNOSTIC_H
#define PARTICLEDIAGNOSTIC_H

#include <array>
#include <string>
#include <vector>

#include "amr/Hierarchy/hierarchy.h"
#include "diagnostics/diagnostics.h"
#include "particlediagnosticstrategy.h"
#include "particlepack.h"


/**
 * @brief The ParticleDiagnostic class is used by AsciiExportStrategy to
 * write data on disk.
 *
 * It encapsulates a container of ParticlePack and a strategy.
 * The strategy will compute or just get a vector of particles, and then
 * put it in a ParticlePack.
 *
 * The compute method loops over the patch Hierarchy.
 */
class ParticleDiagnostic : public Diagnostic
{
protected:
    std::string speciesName_;
    std::vector<ParticlePack> packs_; // one pack per patch
    std::unique_ptr<ParticleDiagnosticStrategy> strategy_;

public:
    ParticleDiagnostic(uint32 id, std::string diagName, std::string speciesName,
                       std::unique_ptr<ParticleDiagnosticStrategy> strat)
        : Diagnostic{id, diagName}
        , speciesName_{speciesName}
        , strategy_{std::move(strat)}
    {
    }

    // routines used to access the diagnostic data per patch by export strat.
    std::vector<ParticlePack> const& getPacks() const { return packs_; }
    void flushPacks();
    std::string const& stratName() const { return strategy_->name(); }

    std::string const& speciesName() const { return speciesName_; }

    virtual void compute(Hierarchy const& hierarchy) final;
};


#endif // PARTICLEDIAGNOSTIC_H
