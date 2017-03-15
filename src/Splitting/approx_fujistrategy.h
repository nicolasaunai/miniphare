#ifndef APPROXFUJISTRATEGY_H
#define APPROXFUJISTRATEGY_H

#include "Splitting/splittingstrategy.h"


class Approx_FujiStrategy : public SplittingStrategy
{
protected:
    double ratioDx_ ;

public:
    Approx_FujiStrategy( const std::string & splitMethod, double ratioDx );

    virtual std::vector<Particle> split(
            double dxL1, uint32 refineFactor,
            uint32 interpOrder,
            const std::vector<Particle> & motherParticles ) const override ;
};

#endif // APPROXFUJISTRATEGY_H
