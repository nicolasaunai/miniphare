#ifndef ORDER3_RF2STRATEGY_H
#define ORDER3_RF2STRATEGY_H

#include "splittingstrategy.h"


class Order3_RF2Strategy : public SplittingStrategy
{
private:
public:
    Order3_RF2Strategy(const std::string& splitMethod);
};

#endif // ORDER3_RF2STRATEGY_H
