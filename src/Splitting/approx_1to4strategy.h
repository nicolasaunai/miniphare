#ifndef APPROX_1TO4STRATEGY_H
#define APPROX_1TO4STRATEGY_H

#include "Splitting/splittingstrategy.h"

class Approx_1to4Strategy : public SplittingStrategy
{
protected:
        double ratioDx_ ;

public:
    Approx_1to4Strategy( const std::string & splitMethod, double ratioDx );

    virtual void split1D(
            const Particle & mother,
            std::vector<Particle> & childParticles ) const override ;
};

#endif // APPROX_1TO4STRATEGY_H
