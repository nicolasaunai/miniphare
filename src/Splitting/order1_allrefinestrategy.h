#ifndef ORDER1_ALLREFINESTRATEGY_H
#define ORDER1_ALLREFINESTRATEGY_H

#include "splittingstrategy.h"


class Order1_RFnStrategy : public SplittingStrategy
{
public:
    Order1_RFnStrategy( const std::string & splitMethod, double ratioDx );

    virtual std::vector<Particle> split(
            const GlobalParams & globalParams  ,
            uint64 totalNbrParticles_          ,
            double dxL1,
            const std::vector<Particle> & motherParticles ) const override ;
};

#endif // ORDER1_ALLREFINESTRATEGY_H
