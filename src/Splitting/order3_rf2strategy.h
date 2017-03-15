#ifndef ORDER3_RF2STRATEGY_H
#define ORDER3_RF2STRATEGY_H

#include "Splitting/splittingstrategy.h"


class Order3_RF2Strategy : public SplittingStrategy
{
public:
    Order3_RF2Strategy( const std::string & splitMethod );

    virtual std::vector<Particle> split(
            double dxL1, uint32 refineFactor,
            uint32 interpOrder,
            const std::vector<Particle> & motherParticles ) const override ;
};

#endif // ORDER3_RF2STRATEGY_H
