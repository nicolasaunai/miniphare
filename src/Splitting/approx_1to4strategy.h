#ifndef APPROX_1TO4STRATEGY_H
#define APPROX_1TO4STRATEGY_H

#include "splittingstrategy.h"

class Split1to4Strategy : public SplittingStrategy
{
protected:
        double ratioDx_ ;

public:
    Split1to4Strategy( const std::string & splitMethod, double ratioDx );

    virtual std::vector<Particle> split(
            const GlobalParams & globalParams  ,
            uint64 totalNbrParticles_          ,
            double dxL1,
            const std::vector<Particle> & motherParticles ) const override ;
};

#endif // APPROX_1TO4STRATEGY_H
