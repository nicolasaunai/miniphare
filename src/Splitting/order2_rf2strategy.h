#ifndef ORDER2_RF2STRATEGY_H
#define ORDER2_RF2STRATEGY_H

#include "Splitting/splittingstrategy.h"


class Order2_RF2Strategy : public SplittingStrategy
{
public:
    Order2_RF2Strategy( const std::string & splitMethod );

    virtual std::vector<Particle> split(
            double dxL1, uint32 refineFactor,
            uint32 interpOrder,
            const std::vector<Particle> & motherParticles ) const override ;
};


#endif // ORDER2_RF2STRATEGY_H
