#ifndef STRATEGYFONCTOR_H
#define STRATEGYFONCTOR_H

#include <memory>

#include "Splitting/splittingstrategy.h"

#include "Splitting/splittingstrategy.h"
#include "Splitting/approx_1to4strategy.h"
#include "Splitting/approx_fujistrategy.h"

#include "Splitting/order1_rf2strategy.h"
#include "Splitting/order2_rf2strategy.h"
#include "Splitting/order3_rf2strategy.h"

#include "Splitting/order1_rfnstrategy.h"
#include "Splitting/ordern_rf2strategy.h"
#include "Splitting/ordern_rf3strategy.h"



class StrategyFonctor
{
protected:
    std::string strategy_ ;

    uint32 refineFactor_ ;
    uint32 interpOrder_ ;

public:
    explicit StrategyFonctor( std::string const & strategy,
                              uint32 refineFactor,
                              uint32 interpOrder )
        : strategy_{strategy}, refineFactor_{refineFactor},
          interpOrder_{interpOrder} {}

    virtual ~StrategyFonctor() = default ;

    virtual std::unique_ptr<SplittingStrategy> createStrategy() const = 0 ;

};



class FonctorApprox1to4 : public StrategyFonctor
{

public:
    explicit FonctorApprox1to4( std::string const & strategy,
                                uint32 refineFactor,
                                uint32 interpOrder )
        : StrategyFonctor(strategy, refineFactor, interpOrder) {}

    virtual std::unique_ptr<SplittingStrategy> createStrategy() const override
    {
        return std::unique_ptr<SplittingStrategy>( new Approx_1to4Strategy(strategy_) );
    }
};


class FonctorApproxFuji : public StrategyFonctor
{

public:
    explicit FonctorApproxFuji( std::string const & strategy,
                                uint32 refineFactor,
                                uint32 interpOrder )
        : StrategyFonctor(strategy, refineFactor, interpOrder) {}

    virtual std::unique_ptr<SplittingStrategy> createStrategy() const override
    {
        return std::unique_ptr<SplittingStrategy>( new Approx_FujiStrategy(strategy_) );
    }
};


class FonctorOrder1_RF2 : public StrategyFonctor
{

public:
    explicit FonctorOrder1_RF2( std::string const & strategy,
                                uint32 refineFactor,
                                uint32 interpOrder )
        : StrategyFonctor(strategy, refineFactor, interpOrder) {}

    virtual std::unique_ptr<SplittingStrategy> createStrategy() const override
    {
        return std::unique_ptr<SplittingStrategy>( new Order1_RF2Strategy(strategy_) );
    }
};


class FonctorOrder2_RF2 : public StrategyFonctor
{

public:
    explicit FonctorOrder2_RF2( std::string const & strategy,
                                uint32 refineFactor,
                                uint32 interpOrder )
        : StrategyFonctor(strategy, refineFactor, interpOrder) {}

    virtual std::unique_ptr<SplittingStrategy> createStrategy() const override
    {
        return std::unique_ptr<SplittingStrategy>( new Order2_RF2Strategy(strategy_) );
    }
};


class FonctorOrder3_RF2 : public StrategyFonctor
{

public:
    explicit FonctorOrder3_RF2( std::string const & strategy,
                                uint32 refineFactor,
                                uint32 interpOrder )
        : StrategyFonctor(strategy, refineFactor, interpOrder) {}

    virtual std::unique_ptr<SplittingStrategy> createStrategy() const override
    {
        return std::unique_ptr<SplittingStrategy>( new Order3_RF2Strategy(strategy_) );
    }
};


class FonctorOrder1_RFn : public StrategyFonctor
{

public:
    explicit FonctorOrder1_RFn( std::string const & strategy,
                                uint32 refineFactor,
                                uint32 interpOrder )
        : StrategyFonctor(strategy, refineFactor, interpOrder) {}

    virtual std::unique_ptr<SplittingStrategy> createStrategy() const override
    {
        return std::unique_ptr<SplittingStrategy>( new Order1_RFnStrategy(strategy_, refineFactor_) );
    }
};


class FonctorOrderN_RF2 : public StrategyFonctor
{

public:
    explicit FonctorOrderN_RF2( std::string const & strategy,
                                uint32 refineFactor,
                                uint32 interpOrder )
        : StrategyFonctor(strategy, refineFactor, interpOrder) {}

    virtual std::unique_ptr<SplittingStrategy> createStrategy() const override
    {
        return std::unique_ptr<SplittingStrategy>( new OrderN_RF2Strategy(strategy_, interpOrder_) );
    }
};


class FonctorOrderN_RF3 : public StrategyFonctor
{

public:
    explicit FonctorOrderN_RF3( std::string const & strategy,
                                uint32 refineFactor,
                                uint32 interpOrder )
        : StrategyFonctor(strategy, refineFactor, interpOrder) {}

    virtual std::unique_ptr<SplittingStrategy> createStrategy() const override
    {
        return std::unique_ptr<SplittingStrategy>( new OrderN_RF3Strategy(strategy_, interpOrder_) );
    }
};



#endif // STRATEGYFONCTOR_H
