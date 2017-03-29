#ifndef APPROX_1TO4STRATEGY_H
#define APPROX_1TO4STRATEGY_H

#include "Splitting/splittingstrategy.h"


class Approx_1to4Strategy : public SplittingStrategy
{
private:
    float jitterX_ ;


public:
    Approx_1to4Strategy( const std::string & splitMethod );

};

#endif // APPROX_1TO4STRATEGY_H
