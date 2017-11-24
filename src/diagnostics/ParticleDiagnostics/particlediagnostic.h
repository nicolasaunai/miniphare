#ifndef PARTICLEDIAGNOSTIC_H
#define PARTICLEDIAGNOSTIC_H

#include <array>
#include <string>
#include <vector>

#include "amr/Hierarchy/hierarchy.h"
#include "diagnostics/diagnostics.h"
#include "particlepack.h"
#include "utilities/particleselector.h"


/**
 * @brief The ParticleDiagnostic class is used by AsciiExportStrategy to
 * write data on disk.
 *
 * It encapsulates a container of ParticlePack and a ParticleSelector (our strategy).
 * The strategy will compute or just get a vector of particles, and then
 * put it in a ParticlePack.
 *
 * The compute method loops over the patch Hierarchy.
 */
class ParticleDiagnostic : public Diagnostic
{
private:
    std::string speciesName_;
    std::vector<ParticlePack> packs_; // one pack per patch
    std::string selectorType_;
    std::unique_ptr<ParticleSelector> selectorPtr_;

    ParticlePack compute_(Patch const& patch);

    void fillPack_(ParticlePack& pack, std::vector<Particle> const& particles,
                   GridLayout const& layout);
    void fillDiagData1D_(ParticlePack& pack, std::vector<Particle> const& particles);

public:
    ParticleDiagnostic(uint32 id, std::string diagName, std::string speciesName,
                       std::string selectorType, std::unique_ptr<ParticleSelector> selector)
        : Diagnostic{id, diagName}
        , speciesName_{speciesName}
        , selectorType_{selectorType}
        , selectorPtr_{std::move(selector)}
    {
    }

    // routines used to access the diagnostic data per patch by export strat.
    std::vector<ParticlePack> const& getPacks() const { return packs_; }
    void flushPacks();
    std::string const& stratName() const { return selectorType_; }

    std::string const& speciesName() const { return speciesName_; }

    virtual void compute(Hierarchy const& hierarchy) final;
};


#endif // PARTICLEDIAGNOSTIC_H
