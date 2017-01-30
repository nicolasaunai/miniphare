#ifndef FARADAYIMPL1D_H
#define FARADAYIMPL1D_H


#include "faraday.h"


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

    ~FaradayImpl1D() = default;

    virtual void operator()(VecField const& E, VecField const& B, VecField& Bnew) override;
};



#endif // FARADAYIMPL1D_H
