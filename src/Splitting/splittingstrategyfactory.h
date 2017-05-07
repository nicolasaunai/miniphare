#ifndef SPLITTINGSTRATEGYFACTORY_H
#define SPLITTINGSTRATEGYFACTORY_H

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "Splitting/splittingstrategy.h"
#include "Splitting/strategyfonctor.h"



class SplittingStrategyFactory
{
private:
    std::string strategy_;
    using StrategyFunctorPtr = std::unique_ptr<StrategyFunctor>;
    std::unordered_map<std::string, StrategyFunctorPtr> strategyMap_;


public:
    SplittingStrategyFactory(std::string const& strategy, uint32 refineFactor, uint32 interpOrder)
        : strategy_{strategy}
    {
        strategyMap_["Approx1to4"]
            = StrategyFunctorPtr(new FonctorApprox1to4(strategy, refineFactor, interpOrder));
        strategyMap_["ApproxFujimoto"]
            = StrategyFunctorPtr(new FonctorApproxFuji(strategy, refineFactor, interpOrder));
        strategyMap_["splitOrder1"]
            = StrategyFunctorPtr(new FonctorOrder1_RF2(strategy, refineFactor, interpOrder));
        strategyMap_["splitOrder2"]
            = StrategyFunctorPtr(new FonctorOrder2_RF2(strategy, refineFactor, interpOrder));
        strategyMap_["splitOrder3"]
            = StrategyFunctorPtr(new FonctorOrder3_RF2(strategy, refineFactor, interpOrder));
        strategyMap_["splitOrder1_RFn"]
            = StrategyFunctorPtr(new FonctorOrder1_RFn(strategy, refineFactor, interpOrder));
        strategyMap_["splitOrderN_RF2"]
            = StrategyFunctorPtr(new FonctorOrderN_RF2(strategy, refineFactor, interpOrder));
        strategyMap_["splitOrderN_RF3"]
            = StrategyFunctorPtr(new FonctorOrderN_RF3(strategy, refineFactor, interpOrder));
    }


    /**
     * @brief createSplittingStrategy is a factory pattern
     */
    std::unique_ptr<SplittingStrategy> createSplittingStrategy()
    {
        std::unique_ptr<SplittingStrategy> strategyPtr{nullptr};

        auto mapIterator = strategyMap_.find(strategy_);
        if (mapIterator != strategyMap_.end())
        {
            StrategyFunctor const& fonctor = *(mapIterator->second);
            strategyPtr                    = fonctor.createStrategy();
            std::cout << "Splitting strategy found: " << mapIterator->first << std::endl;
            std::cout << "With arguments:" << std::endl;
            std::cout << "------> interpOrder  = " << fonctor.interpOrder() << std::endl;
            std::cout << "------> refineFactor = " << fonctor.refineFactor() << std::endl;
        }
        else
        {
            throw std::runtime_error("Error SplittingStrategyFactory: no strategy found !");
        }

        return strategyPtr;
    }
};

#endif // SPLITTINGSTRATEGYFACTORY_H
