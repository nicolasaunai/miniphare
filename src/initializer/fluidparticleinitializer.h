#ifndef FLUIDPARTICLEINITIALIZER_H
#define FLUIDPARTICLEINITIALIZER_H

#include <random>

#include "data/Plasmas/particleinitializer.h"
#include "data/grid/gridlayout.h"
#include "utilities/types.h"


/**
 * @brief FluidParticleInitializer is a ParticleInitializer where
 *        particles are created following density, bulk velocity and temperature
 *        profiles and are distributed according to a locally Mawwellian
 *        distribution function.
 */
class FluidParticleInitializer : public ParticleInitializer
{
private:
    GridLayout layout_;
    std::unique_ptr<ScalarFunction> density_;
    std::unique_ptr<VectorFunction> bulkVelocity_;
    std::unique_ptr<VectorFunction> thermalSpeed_;
    std::unique_ptr<VectorFunction> magneticField_;
    double particleCharge_;
    uint32 nbrParticlePerCell_;
    Basis base_;


    void loadParticles1D_(std::vector<Particle>& particles) const;
    void loadParticles2D_(std::vector<Particle>& particles) const;
    void loadParticles3D_(std::vector<Particle>& particles) const;

public:
    FluidParticleInitializer(GridLayout const& layout,
                             std::unique_ptr<ScalarFunction> densityProfile,
                             std::unique_ptr<VectorFunction> bulkVelocityProfile,
                             std::unique_ptr<VectorFunction> thermalSpeedProfile,
                             uint32 nbrPartPerCell, double particleCharge,
                             Basis base                                    = Basis::Cartesian,
                             std::unique_ptr<VectorFunction> magneticField = nullptr);

    virtual void loadParticles(std::vector<Particle>& particles) const override;

    virtual ~FluidParticleInitializer();
};




#endif // FLUIDPARTICLEINITIALIZER_H
