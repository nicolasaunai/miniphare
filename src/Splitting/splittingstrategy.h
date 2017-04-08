#ifndef SPLITTINGSTRATEGY_H
#define SPLITTINGSTRATEGY_H

#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <cmath>

#include "utilityphare.h"

#include "Plasmas/particles.h"
#include "grid/gridlayout.h"



class SplittingStrategy
{

public:

    SplittingStrategy( std::string name, uint32 nbpts );

    virtual ~SplittingStrategy() = default ;

    std::string const & name() const { return splitMethod_ ; }

    void split1D( const Particle & mother,
                  std::vector<Particle> & childParticles ) const ;

protected:

    std::string splitMethod_ ;

    uint32 nbpts_ ;

    std::vector<int32> child_icellx_ ;
    std::vector<float> child_deltax_ ;

    std::vector<double> child_weights_ ;

    double wtot_ ;

};

#endif // SPLITTINGSTRATEGY_H
