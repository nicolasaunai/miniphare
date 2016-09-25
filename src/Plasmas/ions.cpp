
#include <algorithm>
#include <functional>
#include <memory>
#include <iostream>

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
 Ions::Ions(GridLayout const& layout, IonsInitializer &ionsInitializer):
     layout_{layout},
     rho_    {layout.nx(), layout.ny(), layout.nz(), "_rhoTot"},
     bulkVel_{layout.nx(), layout.ny(), layout.nz(), "_BulkVelTot"}
 {
     uint32 nbrSpecies = ionsInitializer.nbrSpecies;
     std::cout << "Ion Constructor : " << nbrSpecies << std::endl;
     speciesArray_.reserve( nbrSpecies );


     for (uint32 speciesIndex = 0; speciesIndex < nbrSpecies; ++speciesIndex)
     {
         speciesArray_.push_back(Species{layout, ionsInitializer.masses[speciesIndex],
                                             std::move(ionsInitializer.particleInitializers[speciesIndex]),
                                             ionsInitializer.names[speciesIndex]} );

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


 /**
  * @brief Ions::loadParticles loads particles in all ion Species.
  */
 void Ions::loadParticles()
 {
     std::cout << "ion load particles" << std::endl;
     for (Species& species : speciesArray_)
     {
        std::cout << "init a species" << std::endl;
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
     for (Species const& spe : speciesArray_)
     {
         // we assume that rho_ has been reset to 0 before.
         // we sum over all nodes contiguously, including ghosts
         // nodes. This is more efficient and easier to code as we don't
         // have to account for the field dimensionality.

        Field const& rhoSpe = spe.rho();
        std::transform (rho_.begin(), rho_.end(), rhoSpe.begin(), rho_.begin(), std::plus<int>());
     }     
 }




void Ions::computeBulkVelocity()
{
    const uint32 compX=0;
    const uint32 compY=1;
    const uint32 compZ=2;

    Field& vxTot = bulkVel_.component(compX);
    Field& vyTot = bulkVel_.component(compY);
    Field& vzTot = bulkVel_.component(compZ);


    // first we sum the charge density flux of each species
    // sum_s q_s*n_s*v_s
    // we assume the total bulk velocity has been set to zero
    // before, i.e. vxyzTot = 0.
    for (Species const& spe : speciesArray_)
    {
        Field const& rhovX = spe.flux(compX);
        Field const& rhovY = spe.flux(compY);
        Field const& rhovZ = spe.flux(compZ);

        std::transform (vxTot.begin(), vxTot.end(), rhovX.begin(), vxTot.begin(), std::plus<double>());
        std::transform (vyTot.begin(), vyTot.end(), rhovY.begin(), vyTot.begin(), std::plus<double>());
        std::transform (vzTot.begin(), vzTot.end(), rhovZ.begin(), vzTot.begin(), std::plus<double>());
    }


    // now we divide by the total ion density.
    // therefore here we assume that rho_ has been computed before
    // i.e. that the code has called  Ions::computeChargeDensity() before.

    std::transform(vxTot.begin(), vxTot.end(), rho_.begin(), vxTot.begin(), std::divides<double>());
    std::transform(vyTot.begin(), vyTot.end(), rho_.begin(), vyTot.begin(), std::divides<double>());
    std::transform(vzTot.begin(), vzTot.end(), rho_.begin(), vzTot.begin(), std::divides<double>());
}






















