#ifndef ORDERN_RF2STRATEGY_H
#define ORDERN_RF2STRATEGY_H

#include "splittingstrategy.h"


class OrderN_RF2Strategy : public SplittingStrategy
{
public:
    OrderN_RF2Strategy( const std::string & splitMethod, double ratioDx );

    virtual std::vector<Particle> split(
            const GlobalParams & globalParams  ,
            uint64 totalNbrParticles_          ,
            double dxL1,
            const std::vector<Particle> & motherParticles ) const override ;
};


#endif // ORDERN_RF2STRATEGY_H
