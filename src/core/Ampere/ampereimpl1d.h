#ifndef AMPEREIMPL1D_H
#define AMPEREIMPL1D_H

#include "ampere.h"



class AmpereImpl1D : public AmpereImpl, private AmpereImplInternals
{
private:
    Field dxBz_;
    Field dxBy_;


public:
    AmpereImpl1D(GridLayout const& layout);

    AmpereImpl1D(AmpereImpl1D const& source) = delete;
    AmpereImpl1D& operator=(AmpereImpl1D const& source) = delete;

    AmpereImpl1D(AmpereImpl1D&& source) = default;
    AmpereImpl1D& operator=(AmpereImpl1D&& source) = default;

    ~AmpereImpl1D() = default;

    virtual void operator()(VecField const& B, VecField& Jnew) override;
};



#endif // AMPEREIMPL1D_H
