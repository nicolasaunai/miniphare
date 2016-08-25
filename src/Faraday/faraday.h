#ifndef FARADAY_H
#define FARADAY_H

#include <memory>


#include "Vecfield/vecfield.h"
#include "Field/field.h"
#include "grid/gridlayout.h"




// comment here
class FaradayImpl
{
    public:
        virtual void operator()(VecField const& E, VecField const& B, VecField& Bnew) = 0;
        virtual ~FaradayImpl(){};
};




class FaradayImplInternals
{
public:
    FaradayImplInternals(double dt, GridLayout const& layout):dt_{dt},layout_{layout}{}

protected:
    double dt_;
    GridLayout layout_;
};





// comment here
class FaradayImpl1D :  public FaradayImpl, private FaradayImplInternals
{

private:

    Field dxEz_;
    Field dxEy_;


public:
    FaradayImpl1D(double dt, GridLayout const& layout);

    FaradayImpl1D(FaradayImpl1D const& source) = delete;
    FaradayImpl1D& operator=(FaradayImpl1D const& source) = delete;

    FaradayImpl1D(FaradayImpl1D&& source)            = default;
    FaradayImpl1D& operator=(FaradayImpl1D&& source) = default;

    virtual void operator()(VecField const& E, VecField const& B, VecField& Bnew) override;
};





// comment here
class Faraday
{
private:

    //


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
