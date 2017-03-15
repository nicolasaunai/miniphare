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

    inline std::string name() const {return p_name;}

    virtual std::vector<Particle> split(
            double dxL1, uint32 refineFactor,
            uint32 interpOrder,
            const std::vector<Particle> & motherParticles ) const = 0 ;

protected:

    std::string p_name;

};

#endif // SPLITTINGSTRATEGY_H
