#ifndef SPECIES_H
#define SPECIES_H

#include "Field/field.h"
#include "vecfield/vecfield.h"
#include "grid/gridlayout.h"
#include "particles.h"
#include "Initializer/particleinitializer.h"


/**
 * @brief The Species class contains particles and moments of a specific
 * species of Particle.
 */
class Species
{

private:

    GridLayout layout_;
    Field rho_;
    VecField flux_;
    std::vector<Particle> particleArray_;
    std::unique_ptr<ParticleInitializer> particleInitializer_;


public:



    Species(GridLayout const& layout, double mass,
            std::unique_ptr<ParticleInitializer> particleInitializer,
            std::string const& name);

    //Species(GridLayout&& layout, std::string const& name);

    Species(Species const& source) = delete;
    Species& operator=(Species const& source)=delete;

    Species(Species&& source) = default;
    Species& operator=(Species&& source) = default;

    void resetMoments() {rho_.zero(); flux_.zero();}

    Field& rho() {return rho_;}
    Field const& rho() const {return rho_;}

    Field& flux(uint32 iComponent) {return flux_.component(iComponent);}
    Field const& flux(uint32 iComponent) const {return flux_.component(iComponent);}


    void loadParticles();


#if 0
    Field* getChargeDensity();
    Field* getBulkVelocity();
    Field* getFLux();

    void computeChargeDensity();
    void computeFlux();
    void computeBulkVelocity();

#endif

};

#endif // SPECIES_H
