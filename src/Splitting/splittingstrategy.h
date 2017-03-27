#ifndef SPLITTINGSTRATEGY_H
#define SPLITTINGSTRATEGY_H

#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <cmath>

#include "utilityphare.h"

#include "Plasmas/particles.h"



class SplittingStrategy
{
public:

    explicit SplittingStrategy(std::string name);

    virtual ~SplittingStrategy() = default ;

    inline std::string name() const {return p_name;}

    virtual void split1D(
            const Particle & mother,
            std::vector<Particle> & childParticles ) const = 0 ;

protected:

    std::string p_name;

};

#endif // SPLITTINGSTRATEGY_H
