#ifndef ORDERN_RF3STRATEGY_H
#define ORDERN_RF3STRATEGY_H

#include "Splitting/splittingstrategy.h"


class OrderN_RF3Strategy : public SplittingStrategy
{
public:
    OrderN_RF3Strategy( const std::string & splitMethod );

    virtual std::vector<Particle> split(
            double dxL1, uint32 refineFactor,
            uint32 interpOrder,
            const std::vector<Particle> & motherParticles ) const override ;
};


#endif // ORDERN_RF3STRATEGY_H
