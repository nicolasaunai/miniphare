#ifndef ORDER1_ALLREFINESTRATEGY_H
#define ORDER1_ALLREFINESTRATEGY_H

#include "Splitting/splittingstrategy.h"


class Order1_RFnStrategy : public SplittingStrategy
{
public:
    Order1_RFnStrategy( const std::string & splitMethod );

    virtual void split1D(
            const Particle & mother,
            std::vector<Particle> & childParticles ) const override ;
};

#endif // ORDER1_ALLREFINESTRATEGY_H
