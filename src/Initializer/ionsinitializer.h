#ifndef IONSINITIALIZER_H
#define IONSINITIALIZER_H


#include <vector>
#include "particleinitializer.h"
#include "types.h"

class IonsInitializer
{

private:

    std::vector<ParticleInitializer> particleInitializers_;

public:

    uint32 nbrSpecies() const;

    double speciesMass(uint32 speciesIndex) const;

    double speciesCharge(uint32 speciesIndex) const;

    ParticleInitializer const& particleInitializer(uint32 speciesIndex) const;



};

#endif // IONSINITIALIZER_H
