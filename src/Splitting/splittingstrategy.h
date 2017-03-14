#ifndef SPLITTINGSTRATEGY_H
#define SPLITTINGSTRATEGY_H

#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <cmath>

#include "Plasmas/particles.h"



class SplittingStrategy
{
public:

    explicit SplittingStrategy(std::string name);

    inline std::string name() const {return p_name;}

    virtual std::vector<Particle> split(
            const GlobalParams & globalParams  ,
            uint64 totalNbrParticles_          ,
            double dxL1,
            const std::vector<Particle> & motherParticles ) const = 0 ;

protected:

    std::string p_name;

};

#endif // SPLITTINGSTRATEGY_H
