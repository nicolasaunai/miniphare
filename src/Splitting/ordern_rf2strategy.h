#ifndef ORDERN_RF2STRATEGY_H
#define ORDERN_RF2STRATEGY_H

#include "Splitting/splittingstrategy.h"


class OrderN_RF2Strategy : public SplittingStrategy
{
private:
    uint32 nbpts_ ;

    std::vector<int32> child_icellx_ ;
    std::vector<double> child_weights_ ;

    double wtot_ ;

public:

    OrderN_RF2Strategy( const std::string & splitMethod,
                        uint32 refineFactor,
                        uint32 interpOrder ) ;

    virtual void split1D(
            const Particle & mother,
            std::vector<Particle> & childParticles ) const override ;
};


#endif // ORDERN_RF2STRATEGY_H
