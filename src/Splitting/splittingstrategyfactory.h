#ifndef SPLITTINGSTRATEGYFACTORY_H
#define SPLITTINGSTRATEGYFACTORY_H

#include <string>
#include <memory>
#include <utility>
#include <map>

#include "Splitting/splittingstrategy.h"

#include "Splitting/approx_1to4strategy.h"
#include "Splitting/approx_fujistrategy.h"

#include "Splitting/order1_rf2strategy.h"
#include "Splitting/order2_rf2strategy.h"
#include "Splitting/order3_rf2strategy.h"

#include "Splitting/order1_rfnstrategy.h"
#include "Splitting/ordern_rf2strategy.h"
#include "Splitting/ordern_rf3strategy.h"



static std::map<std::string, SplittingStrategy* > strategyMap_ ;


class SplittingStrategyFactory
{
private:

    static void buildStrategyMap_( const std::string & splitMethod,
                                   uint32 interpOrder, uint32 refineFactor )
    {
        strategyMap_.clear() ;

        strategyMap_["ApproxFujimoto"] = new Approx_FujiStrategy(splitMethod) ;
        strategyMap_["Approx1to4"] = new Approx_1to4Strategy(splitMethod) ;
        strategyMap_["splitOrder1"] = new Order1_RF2Strategy(splitMethod) ;
        strategyMap_["splitOrder2"] = new Order2_RF2Strategy(splitMethod) ;
        strategyMap_["splitOrder3"] = new Order3_RF2Strategy(splitMethod) ;
        strategyMap_["splitOrder1_RFn"] = new Order1_RFnStrategy(splitMethod, refineFactor) ;
        strategyMap_["splitOrderN_RF2"] = new OrderN_RF2Strategy(splitMethod, interpOrder) ;
        strategyMap_["splitOrderN_RF3"] = new OrderN_RF3Strategy(splitMethod, interpOrder) ;
    }


public:

    static std::unique_ptr<SplittingStrategy>
    createSplittingStrategy( const std::string & splitMethod,
                             uint32 interpOrder, uint32 refineFactor )
    {

        buildStrategyMap_(splitMethod, interpOrder, refineFactor) ;

        auto strategyPtr = strategyMap_.find(splitMethod) ;
        if( strategyPtr != strategyMap_.end() )
        {
            std::cout << "Splitting strategy found: " << strategyPtr->first << std::endl ;
        }
        else {
            throw std::runtime_error("Error SplittingStrategyFactory: no strategy found !");
        }

        return std::unique_ptr<SplittingStrategy>( strategyPtr->second ) ;
    }



};

#endif // SPLITTINGSTRATEGYFACTORY_H
