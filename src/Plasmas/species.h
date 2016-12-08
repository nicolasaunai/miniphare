#ifndef SPECIES_H
#define SPECIES_H

#include "Field/field.h"
#include "vecfield/vecfield.h"
#include "grid/gridlayout.h"
#include "particles.h"
#include "Initializer/particleinitializer.h"

#include "Projector/projector.h"
#include "Interpolator/interpolator.h"


/**
 * @brief The Species class contains particles and moments of a specific
 * species of Particle. A Species is a heavy object (it contains particles)
 * and is therefore a move only object.
 */
class Species
{

private:

    double mass_;
    std::string name_;
    GridLayout layout_;
    Field rho_;
    VecField flux_;
    std::vector<Particle> particleArray_;
    std::unique_ptr<ParticleInitializer> particleInitializer_;


public:

    Species(GridLayout const& layout, double mass,
            std::unique_ptr<ParticleInitializer> particleInitializer,
            std::string const& name);   

    Species(Species const& source) = delete;
    Species& operator=(Species const& source)=delete;

    Species(Species&& source) = default;
    Species& operator=(Species&& source) = default;

    void resetMoments() {rho_.zero(); flux_.zero();}

    Field& rho() {return rho_;}
    Field const& rho() const {return rho_;}

    Field& flux(uint32 iComponent) {return flux_.component(iComponent);}
    Field const& flux(uint32 iComponent) const {return flux_.component(iComponent);}

    std::vector<Particle>& particles() {return particleArray_;}
    std::vector<Particle> const& particles() const {return particleArray_;}

    void loadParticles();

    void compute1DChargeDensityAndFlux( Projector & project );
    void compute1DFieldsAtParticles( Interpolator & interp, \
                                   VecField const & E , \
                                   VecField const & B );

};

#endif // SPECIES_H
