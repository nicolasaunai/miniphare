#ifndef FARADAY_H
#define FARADAY_H

#include <memory>


#include "Vecfield/vecfield.h"
#include "grid/gridlayout.h"



class FaradayImpl
{
    public:
        virtual void operator()(VecField const& E, VecField const& B, VecField& Bnew,
                            double dt, std::vector<double> dxdydz) = 0;
};



class FaradayImpl1D :  public FaradayImpl
{
public:
    FaradayImpl1D()                            = default;

    FaradayImpl1D(FaradayImpl1D const& source) = delete;
    FaradayImpl1D& operator=(FaradayImpl1D const& source) = delete;

    FaradayImpl1D(FaradayImpl1D&& source)            = default;
    FaradayImpl1D& operator=(FaradayImpl1D&& source) = default;

    virtual void operator()(VecField const& E, VecField const& B, VecField& Bnew,
                        double dt, std::vector<double> dxdydz) override;
};




class Faraday
{
private:
    double dt_;
    std::vector<double> dxdydz_;
    uint32 nbDims_;
    GridLayout layout_;
    std::unique_ptr<FaradayImpl> implPtr_;


public:

    // TODO : faraday object should take start and end "iterators"
    // those are given by SOlver which takes them from GridLayout
    explicit Faraday(double dt, GridLayout const& layout);


    Faraday(Faraday const& source) = delete;
    Faraday& operator=(Faraday const& source) = delete;

    Faraday(Faraday&& source)            = default;
    Faraday& operator=(Faraday&& source) = default;



    void operator()(VecField const& E,
                    VecField const& B,
                    VecField& Bnew);

};



#endif // FARADAY_H
