#ifndef ORDER3_RF2STRATEGY_H
#define ORDER3_RF2STRATEGY_H

#include "splittingstrategy.h"


class Order3Strategy : public SplittingStrategy
{
public:
    Order3Strategy( const std::string & splitMethod, double ratioDx );

    virtual std::vector<Particle> split(
            const GlobalParams & globalParams  ,
            uint64 totalNbrParticles_          ,
            double dxL1,
            const std::vector<Particle> & motherParticles ) const override ;
};

#endif // ORDER3_RF2STRATEGY_H
