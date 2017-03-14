#ifndef APPROXFUJISTRATEGY_H
#define APPROXFUJISTRATEGY_H

#include "splittingstrategy.h"


class Method1Strategy : public SplittingStrategy
{
protected:
    double ratioDx_ ;

public:
    Method1Strategy( const std::string & splitMethod, double ratioDx );

    virtual std::vector<Particle> split(
            const GlobalParams & globalParams  ,
            uint64 totalNbrParticles_          ,
            double dxL1,
            const std::vector<Particle> & motherParticles ) const override ;
};

#endif // APPROXFUJISTRATEGY_H
