#ifndef ORDER2_RF2STRATEGY_H
#define ORDER2_RF2STRATEGY_H

#include "splittingstrategy.h"


class Order2_RF2Strategy : public SplittingStrategy
{
private:
public:
    Order2_RF2Strategy(const std::string& splitMethod);
};


#endif // ORDER2_RF2STRATEGY_H
