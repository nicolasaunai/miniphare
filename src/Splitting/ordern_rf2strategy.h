#ifndef ORDERN_RF2STRATEGY_H
#define ORDERN_RF2STRATEGY_H

#include "Splitting/splittingstrategy.h"


class OrderN_RF2Strategy : public SplittingStrategy
{
private:



public:

    OrderN_RF2Strategy( const std::string & splitMethod,
                        uint32 interpOrder ) ;

};


#endif // ORDERN_RF2STRATEGY_H
