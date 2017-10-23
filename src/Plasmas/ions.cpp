
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>

#include "ions.h"


/**
  * @brief Ions::Ions Build an Ions object from a grid layout and IonInitializer
  * Building Ions does not mean loading particles, just building the skeletton:
  * moment fields, array of species is constructed (i.e. they are given the various
  * particle properties: particleInitializer, mass, etc.)
  *
  *
  * @param layout: GridLayout required for Ions moment fields to allocated
  * @param ionsInitializer: used to initialize ion species, i.e. give them what they
  * need to be able to load particles correctly. IonInitializer enables Ions objects
  * to be built from different contexts: either the IonInitializer is constructed from
  * a user input factory, or in the course of the simulation from the MLMD manager.
  */
Ions::Ions(GridLayout const& layout, std::unique_ptr<IonsInitializer> ionInitializer)

    : speciesArray_{}
    , rho_{layout.allocSize(HybridQuantity::rho), HybridQuantity::rho, "_rhoTot"}
    , bulkVel_{layout.allocSize(HybridQuantity::V),
               layout.allocSize(HybridQuantity::V),
               layout.allocSize(HybridQuantity::V),
               {{HybridQuantity::V, HybridQuantity::V, HybridQuantity::V}},
               "_bulkVelTot"}
{
    uint32 nbrSpecies = ionInitializer->nbrSpecies;
    std::cout << "Building Ions with " << nbrSpecies << " species" << std::endl;

    speciesArray_.reserve(nbrSpecies);


    for (uint32 speciesIndex = 0; speciesIndex < nbrSpecies; ++speciesIndex)
    {
        speciesArray_.push_back(
            Species{layout, ionInitializer->masses[speciesIndex],
                    std::move(ionInitializer->particleInitializers[speciesIndex]),
                    ionInitializer->names[speciesIndex]});

        name2ID_.insert({ionInitializer->names[speciesIndex], speciesIndex});
    }
}




Species const& Ions::species(uint32 index) const
{
    return speciesArray_[index];
}




Species& Ions::species(uint32 index)
{
    return speciesArray_[index];
}


Species const& Ions::species(std::string name) const
{
    return speciesArray_[speciesID(name)];
}


Species& Ions::species(std::string name)
{
    return speciesArray_[speciesID(name)];
}




/**
 * @brief Ions::loadParticles loads particles in all ion Species.
 */
void Ions::loadParticles()
{
    std::cout << "Ions > loading particles..." << std::endl;

    for (Species& species : speciesArray_)
    {
        std::cout << "... species : " << species.name() << std::endl;
        species.loadParticles();
    }
}



/**
 * @brief Ions::computeChargeDensity calculates the TOTAL charge density
 * the method does not go over particles, it just takes the Species
 * charge densities (assumed already calculated) and calculate the sum
 */
void Ions::computeChargeDensity()
{
    // reset charge density to zero
    rho_.zero();

    for (Species const& spe : speciesArray_)
    {
        // we sum over all nodes contiguously, including ghosts
        // nodes. This is more efficient and easier to code as we don't
        // have to account for the field dimensionality.

        Field const& rhoSpe = spe.rho();
        std::transform(rho_.begin(), rho_.end(), rhoSpe.begin(), rho_.begin(), std::plus<double>());
    }
}




void Ions::computeBulkVelocity()
{
    // reset bulk velocity to 0
    bulkVel_.zero();


    Field& vxTot = bulkVel_.component(VecField::VecX);
    Field& vyTot = bulkVel_.component(VecField::VecY);
    Field& vzTot = bulkVel_.component(VecField::VecZ);


    // first we sum the charge density flux of each species
    // sum_s q_s*n_s*v_s
    for (Species const& spe : speciesArray_)
    {
        Field const& rhovX = spe.flux(VecField::VecX);
        Field const& rhovY = spe.flux(VecField::VecY);
        Field const& rhovZ = spe.flux(VecField::VecZ);

        std::transform(vxTot.begin(), vxTot.end(), rhovX.begin(), vxTot.begin(),
                       std::plus<double>());
        std::transform(vyTot.begin(), vyTot.end(), rhovY.begin(), vyTot.begin(),
                       std::plus<double>());
        std::transform(vzTot.begin(), vzTot.end(), rhovZ.begin(), vzTot.begin(),
                       std::plus<double>());
    }


    // now we divide by the total ion density.
    // therefore here we assume that rho_ has been computed before
    // i.e. that the code has called  Ions::computeChargeDensity() before.


    std::transform(vxTot.begin(), vxTot.end(), rho_.begin(), vxTot.begin(), std::divides<double>());
    std::transform(vyTot.begin(), vyTot.end(), rho_.begin(), vyTot.begin(), std::divides<double>());
    std::transform(vzTot.begin(), vzTot.end(), rho_.begin(), vzTot.begin(), std::divides<double>());
}



uint32 Ions::population() const
{
    uint32 popTot = 0;

    uint32 ispe = 0;
    for (Species const& spe : speciesArray_)
    {
        // popTot += spe.particles().size();
        std::cout << "Species " << ispe << ", population = " << spe.particles().size() << std::endl;
        ++ispe;
    }

    return popTot;
}
