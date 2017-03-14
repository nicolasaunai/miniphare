#ifndef MAXWELLIANSTRATEGY_H
#define MAXWELLIANSTRATEGY_H

#include "distribstrategy.h"

class MaxwellianStrategy : public DistribStrategy
{
public:
    MaxwellianStrategy();

    virtual ~MaxwellianStrategy() override;

    virtual void draw(std::vector<double>& array,
                      uint64 nbElemts,
                      double mean,
                      double std) const override;

    virtual void draw(double & value,
                      double mean,
                      double std) const override;

    //! Generate a 1D Gaussian G(0,1) distribution
    //! via the Box-Muller method
    void aleat_gaussBox( std::vector<double> &array,
                         unsigned int nbElemts )  ;

    //! Generate a 1D Gaussian G(0,1) distribution
    //! via the central limit theorem
    void aleat_gauss2( std::vector<double> &array,
                       unsigned int nbElemts,
                       unsigned int Nmix ) ;

};

#endif // MAXWELLIANSTRATEGY_H
