#ifndef SPLITTINGSTRATEGYFACTORY_H
#define SPLITTINGSTRATEGYFACTORY_H

#include <string>
#include <memory>
#include <utility>


#include "Splitting/splittingstrategy.h"

#include "Splitting/approx_1to4strategy.h"
#include "Splitting/approx_fujistrategy.h"

#include "Splitting/order1_rf2strategy.h"
#include "Splitting/order2_rf2strategy.h"
#include "Splitting/order3_rf2strategy.h"

#include "Splitting/order1_rfnstrategy.h"
#include "Splitting/ordern_rf2strategy.h"
#include "Splitting/ordern_rf3strategy.h"




class SplittingStrategyFactory
{
public:

    static std::unique_ptr<SplittingStrategy>
    createSplittingStrategy(const std::string & splitMethod,
                            double ratioDx )
    {

        bool stratOkOrNot = false ;
        std::unique_ptr<SplittingStrategy> split_strategy ;

        if( splitMethod.compare("ApproxFujimoto")==0 )
        {
            stratOkOrNot = true ;
            split_strategy = std::unique_ptr<SplittingStrategy> ( new Approx_FujiStrategy{splitMethod, ratioDx} );
        }

        if( splitMethod.compare("Approx1to4")==0 )
        {
            stratOkOrNot = true ;
            split_strategy = std::unique_ptr<SplittingStrategy> ( new Approx_1to4Strategy{splitMethod, ratioDx} );
        }

        if( splitMethod.compare("splitOrder1")==0 )
        {
            stratOkOrNot = true ;
            split_strategy = std::unique_ptr<SplittingStrategy> ( new Order1_RF2Strategy{splitMethod} );
        }

        if( splitMethod.compare("splitOrder2")==0 )
        {
            stratOkOrNot = true ;
            split_strategy = std::unique_ptr<SplittingStrategy> ( new Order2_RF2Strategy{splitMethod} );
        }

        if( splitMethod.compare("splitOrder3")==0 )
        {
            stratOkOrNot = true ;
            split_strategy = std::unique_ptr<SplittingStrategy> ( new Order3_RF2Strategy{splitMethod} );
        }

        if( splitMethod.compare("splitOrder1_RFn")==0 )
        {
            stratOkOrNot = true ;
            split_strategy = std::unique_ptr<SplittingStrategy> ( new Order1_RFnStrategy{splitMethod} );
        }

        if( splitMethod.compare("splitOrderN_RF2")==0 )
        {
            stratOkOrNot = true ;
            split_strategy = std::unique_ptr<SplittingStrategy> ( new OrderN_RF2Strategy{splitMethod} );
        }

        if( splitMethod.compare("splitOrderN_RF3")==0 )
        {
            stratOkOrNot = true ;
            split_strategy = std::unique_ptr<SplittingStrategy> ( new OrderN_RF3Strategy{splitMethod} );
        }

        if(!stratOkOrNot)
        {
             throw std::runtime_error("Error : SplittingStrategyFactory - ");
        }

        return split_strategy ;
    }



};

#endif // SPLITTINGSTRATEGYFACTORY_H
