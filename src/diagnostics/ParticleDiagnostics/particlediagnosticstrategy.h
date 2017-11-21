#ifndef PARTICLEDIAGNOSTICSTRATEGY_H
#define PARTICLEDIAGNOSTICSTRATEGY_H


#include "amr/Patch/patch.h"
#include "data/Plasmas/particles.h"
#include "data/grid/gridlayout.h"
#include "particlepack.h"

#include "utilities/particleselector.h"

/**
 * @brief The ParticleDiagnosticStrategy class is a base class used
 * by FieldDiagnostic::compute() to fill a particle array with particles.
 */
class ParticleDiagnosticStrategy
{
private:
    std::string stratName_;
    std::string speciesName_;
    std::unique_ptr<ParticleSelector> selectorPtr_;


    void fillDiagData1D_(ParticlePack& pack, std::vector<Particle> const& particles);

    void fillPack_(ParticlePack& pack, std::vector<Particle> const& particles,
                   GridLayout const& layout);

public:
    ParticleDiagnosticStrategy(std::string const& selectorType, std::string const& speciesName,
                               std::unique_ptr<ParticleSelector> selector)
        : stratName_{selectorType}
        , speciesName_{speciesName}
        , selectorPtr_{std::move(selector)}
    {
    }

    /**
     * @brief compute is overriden by a concrete ParticleDiagnosticStrategy class
     */
    ParticlePack compute(Patch const& patch);

    std::string const& name() const { return stratName_; }
};


#endif // PARTICLEDIAGNOSTICSTRATEGY_H
