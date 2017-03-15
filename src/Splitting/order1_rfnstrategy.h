#ifndef ORDER1_ALLREFINESTRATEGY_H
#define ORDER1_ALLREFINESTRATEGY_H

#include "Splitting/splittingstrategy.h"


class Order1_RFnStrategy : public SplittingStrategy
{
public:
    Order1_RFnStrategy( const std::string & splitMethod );

    virtual std::vector<Particle> split(
            double dxL1, uint32 refineFactor,
            uint32 interpOrder,
            const std::vector<Particle> & motherParticles ) const override ;
};

#endif // ORDER1_ALLREFINESTRATEGY_H
