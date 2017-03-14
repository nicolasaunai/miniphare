#ifndef ORDER2_RF2STRATEGY_H
#define ORDER2_RF2STRATEGY_H

#include "splittingstrategy.h"


class Order2Strategy : public SplittingStrategy
{
public:
    Order2Strategy( const std::string & splitMethod, double ratioDx );

    virtual std::vector<Particle> split(
            const GlobalParams & globalParams  ,
            uint64 totalNbrParticles_          ,
            double dxL1,
            const std::vector<Particle> & motherParticles ) const override ;
};


#endif // ORDER2_RF2STRATEGY_H
