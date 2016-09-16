#ifndef IONSINITIALIZER_H
#define IONSINITIALIZER_H


#include <vector>
#include <string>

#include "particleinitializer.h"
#include "types.h"


/**
 * @brief IonInitializer is need for Ions construction so that Ions
 *       may be initialized (species built and particles loaded)
 *
 *  IonInitializer objects are typically created by InitializerFactory objects
 *  which build and initialize them either from input user parameters, when
 *  the simulation is being initialized, or from the MLMD manager, when a
 *  new Ion needs to be instanciated in the PatchData of a new Patch.
 *
 * It contains all necessary information of Ions to build their Species. In
 * particular, it gives access for each Species to
 *
 *  - its ParticleInitializer.
 *  - the mass of its particles
 *  - its name
 *
 *
 * IonsInitializer objects are default constructible.
 *
 */
class IonsInitializer
{

private:

    std::vector< ParticleInitializer > particleInitializers_;
    std::vector<double> masses_;
    std::vector<std::string> names_;


public:

    explicit IonsInitializer(uint32 nbrSpecies);


    uint32 nbrSpecies() const {return static_cast<uint32>(names_.size()); }


    double speciesMass(uint32 speciesIndex) const {return masses_[speciesIndex];}

    std::string speciesName(uint32 speciesIndex) const {return names_[speciesIndex];}

    ParticleInitializer const& particleInitializer(uint32 speciesIndex) const
    {return particleInitializers_[speciesIndex];}



};



#endif // IONSINITIALIZER_H
