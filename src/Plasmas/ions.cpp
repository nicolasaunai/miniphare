
#include <algorithm>
#include <functional>

#include "ions.h"


/**
  * @brief Ions::Ions Build an Ions object from a grid layout
  * @param layout: GridLayout required for Ions to allocated
  */
 Ions::Ions(GridLayout const& layout, const IonsInitializer &ionsInitializer):
     layout_{layout},
     rho_    {layout.nx(), layout.ny(), layout.nz(), "_rhoTot"},
     bulkVel_{layout.nx(), layout.ny(), layout.nz(), "_BulkVelTot"}
 {

     uint32 nbrSpecies = ionsInitializer.nbrSpecies;
     speciesArray_.reserve( nbrSpecies );

     for (uint32 speciesIndex = 0; speciesIndex < nbrSpecies; ++speciesIndex)
     {
         speciesArray_.push_back( Species { layout,
                                            ionsInitializer.masses[speciesIndex],
                                            ionsInitializer.particleInitializers[speciesIndex],
                                            ionsInitializer.names[speciesIndex]
                                          }
                                 );
     }
 }




/*
 Ions::Ions(GridLayout&& layout)
     : layout_{std::move(layout)}, // layout is DEAD, use layout_ now
       rho_    {layout_.nx(), layout_.ny(), layout_.nz(), "_rhoTot"},
       bulkVel_{layout_.nx(), layout_.ny(), layout_.nz(), "_BulkVelTot"}
{
}
*/



 Species const& Ions::species(uint32 index) const
 {
    return speciesArray_[index];
 }




 Species& Ions::species(uint32 index)
 {
    return speciesArray_[index];
 }



 void Ions::loadParticles()
 {
     for (Species& species : speciesArray_)
     {
        species.loadParticles();
     }
 }




 void Ions::computeChargeDensity()
 {
     for (Species const& spe : speciesArray_)
     {
         // we assume that rho_ has been reset to 0 before.
         // we sum over all nodes contiguously, including ghosts and possible
         // padding nodes. This is more efficient and easier to code as we don't
         // have to account for the field dimensionality.

        Field const& rhoSpe = spe.rho();
        std::transform (rho_.begin(), rho_.end(), rhoSpe.begin(), rho_.begin(), std::plus<int>());
     }
     double invNbrSpecies = 1./static_cast<double>(speciesArray_.size());
     for (double rho: rho_) rho * invNbrSpecies;
 }











