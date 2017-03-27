#ifndef APPROXFUJISTRATEGY_H
#define APPROXFUJISTRATEGY_H

#include "Splitting/splittingstrategy.h"


class Approx_FujiStrategy : public SplittingStrategy
{
protected:
    double ratioDx_ ;

public:
    Approx_FujiStrategy( const std::string & splitMethod, double ratioDx );

    virtual void split1D(
            const Particle & mother,
            std::vector<Particle> & childParticles ) const override ;
};

#endif // APPROXFUJISTRATEGY_H
