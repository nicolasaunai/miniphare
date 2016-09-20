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
 *  IonInitializer objects are created by InitializerFactory objects
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

public:

    // TODO ParticleInitializer should have a noexcept move Ctor
    // so that this vector can move push_back() them in

    std::vector< std::unique_ptr<ParticleInitializer> > particleInitializers;
    std::vector<double> masses;
    std::vector<std::string> names;
    uint32 nbrSpecies;


};



#endif // IONSINITIALIZER_H
