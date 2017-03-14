#ifndef DISTRIBSTRATEGY_H
#define DISTRIBSTRATEGY_H

#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <cmath>

#include "types.h"


class DistribStrategy
{
public:
    DistribStrategy();
    explicit DistribStrategy(std::string name);

    virtual ~DistribStrategy();

    inline std::string name() const {return p_name;}

    virtual void draw(std::vector<double>& array,
                      uint64 nbElemts,
                      double mean,
                      double std) const = 0;

    virtual void draw(double & value,
                      double mean,
                      double std) const = 0;

protected:
    std::string p_name;



};

#endif // DISTRIBSTRATEGY_H
