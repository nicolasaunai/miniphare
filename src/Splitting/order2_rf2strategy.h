#ifndef ORDER2_RF2STRATEGY_H
#define ORDER2_RF2STRATEGY_H

#include "Splitting/splittingstrategy.h"


class Order2_RF2Strategy : public SplittingStrategy
{
private:
    const uint32 refineFactor_ = 2 ;
    const uint32 interpOrder_ = 2 ;

public:
    Order2_RF2Strategy( const std::string & splitMethod );

    virtual void split1D(
            const Particle & mother,
            std::vector<Particle> & childParticles ) const override ;
};


#endif // ORDER2_RF2STRATEGY_H
