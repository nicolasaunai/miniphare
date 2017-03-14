#ifndef DISTRIBGENERATOR_H
#define DISTRIBGENERATOR_H

#include <vector>
#include <memory>

#include "distribstrategy.h"


class DistribGenerator
{
public:
    DistribGenerator();

    void setStrategy(std::shared_ptr<DistribStrategy> strategy);

    //TODO should be template method with 'mean,std' encapusulated
    // in a type known only by the strategy
    void draw(std::vector<double>& array,
              uint64 nbElements,
              double mean,
              double std) const
    {
        this->p_strategy->draw(array,nbElements,mean,std);
    }

    void draw(double & value,
              double mean,
              double std) const
    {
        this->p_strategy->draw(value, mean, std);
    }

    inline std::string strategyName() const
    {
        return p_strategy->name();
    }


private:
    std::shared_ptr<DistribStrategy> p_strategy;
};


#endif // DISTRIBGENERATOR_H
