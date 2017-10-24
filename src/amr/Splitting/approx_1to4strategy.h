#ifndef APPROX_1TO4STRATEGY_H
#define APPROX_1TO4STRATEGY_H

#include "splittingstrategy.h"


class Approx_1to4Strategy : public SplittingStrategy
{
private:
    float shiftX_;


public:
    Approx_1to4Strategy(const std::string& splitMethod);
};

#endif // APPROX_1TO4STRATEGY_H
