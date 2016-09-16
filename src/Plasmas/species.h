#ifndef SPECIES_H
#define SPECIES_H

#include "Field/field.h"
#include "grid/gridlayout.h"
#include "particles.h"
#include "Initializer/particleinitializer.h"



class Species
{

private:

    GridLayout layout_;
    Field rho_;
    Field bulkVel_;
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




    void resetMoments() {rho_.zero(); bulkVel_.zero();}

    Field& rho() {return rho_;}

    Field const& rho() const {return rho_;}




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
