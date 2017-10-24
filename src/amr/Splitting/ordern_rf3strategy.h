#ifndef ORDERN_RF3STRATEGY_H
#define ORDERN_RF3STRATEGY_H

#include "splittingstrategy.h"


class OrderN_RF3Strategy : public SplittingStrategy
{
private:
public:
    OrderN_RF3Strategy(const std::string& splitMethod, uint32 interpOrder);
};


#endif // ORDERN_RF3STRATEGY_H
