#ifndef SPLITTINGSTRATEGYFACTORY_H
#define SPLITTINGSTRATEGYFACTORY_H

#include <string>
#include <memory>
#include <utility>
#include <unordered_map>

#include "Splitting/splittingstrategy.h"

#include "Splitting/strategyfonctor.h"



class SplittingStrategyFactory
{
private:
    std::string strategy_ ;

    std::unordered_map<std::string, std::unique_ptr<StrategyFonctor>> strategyMap_ ;


public:

    SplittingStrategyFactory( std::string const & strategy,
                              uint32 refineFactor,
                              uint32 interpOrder )
        : strategy_{strategy}
    {
        strategyMap_["Approx1to4"]      = std::unique_ptr<StrategyFonctor>(new FonctorApprox1to4(strategy, refineFactor, interpOrder)) ;
        strategyMap_["ApproxFujimoto"]  = std::unique_ptr<StrategyFonctor>(new FonctorApproxFuji(strategy, refineFactor, interpOrder)) ;
        strategyMap_["splitOrder1"]     = std::unique_ptr<StrategyFonctor>(new FonctorOrder1_RF2(strategy, refineFactor, interpOrder)) ;
        strategyMap_["splitOrder2"]     = std::unique_ptr<StrategyFonctor>(new FonctorOrder2_RF2(strategy, refineFactor, interpOrder)) ;
        strategyMap_["splitOrder3"]     = std::unique_ptr<StrategyFonctor>(new FonctorOrder3_RF2(strategy, refineFactor, interpOrder)) ;
        strategyMap_["splitOrder1_RFn"] = std::unique_ptr<StrategyFonctor>(new FonctorOrder1_RFn(strategy, refineFactor, interpOrder)) ;
        strategyMap_["splitOrderN_RF2"] = std::unique_ptr<StrategyFonctor>(new FonctorOrderN_RF2(strategy, refineFactor, interpOrder)) ;
        strategyMap_["splitOrderN_RF3"] = std::unique_ptr<StrategyFonctor>(new FonctorOrderN_RF3(strategy, refineFactor, interpOrder)) ;
    }


    /**
     * @brief createSplittingStrategy is a factory pattern
     *
     *
     * @return
     */
    std::unique_ptr<SplittingStrategy> createSplittingStrategy()
    {
        std::unique_ptr<SplittingStrategy> strategyPtr{nullptr} ;

        auto mapIterator = strategyMap_.find(strategy_) ;
        if( mapIterator != strategyMap_.end() )
        {
            StrategyFonctor const & fonctor = *(mapIterator->second) ;
            strategyPtr = fonctor.createStrategy() ;
            std::cout << "Splitting strategy found: " << mapIterator->first << std::endl ;
            std::cout << "With arguments:" << std::endl ;
            std::cout << "------> interpOrder  = " << fonctor.interpOrder() << std::endl ;
            std::cout << "------> refineFactor = " << fonctor.refineFactor() << std::endl ;
        }
        else {
            throw std::runtime_error("Error SplittingStrategyFactory: no strategy found !");
        }

        return strategyPtr ;
    }

};

#endif // SPLITTINGSTRATEGYFACTORY_H
