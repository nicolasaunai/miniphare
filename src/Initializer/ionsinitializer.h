#ifndef IONSINITIALIZER_H
#define IONSINITIALIZER_H


#include <vector>
#include <string>

#include "particleinitializer.h"
#include "types.h"

class IonsInitializer
{

private:

    std::vector<ParticleInitializer> particleInitializers_;
    uint32 nbrSpecies_;
    std::vector<double> masses_;
    std::vector<std::string> names_;


public:

    uint32 nbrSpecies() const {return nbrSpecies_;}

    double speciesMass(uint32 speciesIndex) const {return masses_[speciesIndex];}

    std::string speciesName(uint32 speciesIndex) const {return names_[speciesIndex];}

    ParticleInitializer const& particleInitializer(uint32 speciesIndex) const
    {return particleInitializers_[speciesIndex];}



};

#endif // IONSINITIALIZER_H
