#ifndef ORDER1_ALLREFINESTRATEGY_H
#define ORDER1_ALLREFINESTRATEGY_H

#include "splittingstrategy.h"


class Order1_RFnStrategy : public SplittingStrategy
{
private:
public:
    Order1_RFnStrategy(const std::string& splitMethod, uint32 refineFactor);
};

#endif // ORDER1_ALLREFINESTRATEGY_H
