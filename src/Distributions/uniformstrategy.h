#ifndef UNIFORMSTRATEGY_H
#define UNIFORMSTRATEGY_H

#include "distribstrategy.h"


class UniformStrategy : public DistribStrategy
{
public:
    UniformStrategy();

    virtual ~UniformStrategy() override;

    virtual void draw(std::vector<double>& array,
                      uint64 nbElemts,
                      double mean,
                      double std) const override;

    virtual void draw(double & value,
                      double mean,
                      double std) const override;

};


#endif // UNIFORMSTRATEGY_H
