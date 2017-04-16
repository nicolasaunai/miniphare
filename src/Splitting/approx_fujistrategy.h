#ifndef APPROXFUJISTRATEGY_H
#define APPROXFUJISTRATEGY_H

#include "Splitting/splittingstrategy.h"


class Approx_FujiStrategy : public SplittingStrategy
{
protected:
    float jitterX_ ;

public:
    Approx_FujiStrategy( const std::string & splitMethod );

};

#endif // APPROXFUJISTRATEGY_H
