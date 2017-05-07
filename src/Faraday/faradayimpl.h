#ifndef FARADAYIMPL_H
#define FARADAYIMPL_H

#include "Field/field.h"
#include "grid/gridlayout.h"
#include "vecfield/vecfield.h"




/**
 * @brief Interface for concrete Faraday implementations (1D, 2D, 3D)
 */
class FaradayImpl
{
public:
    virtual ~FaradayImpl() = default;

    virtual void operator()(VecField const& E, VecField const& B, VecField& Bnew) = 0;
};




/**
 * @brief The FaradayImplInternals class factorizes details of implementation
 *  for the concrete derived classes inheriting from FaradayImpl.
 *
 *  The class encapsulates the time step and the GridLayout
 */
class FaradayImplInternals
{
public:
    FaradayImplInternals(double dt, GridLayout const& layout)
        : dt_{dt}
        , layout_{layout}
    {
    }

protected:
    double dt_;
    GridLayout layout_;
};



#endif // FARADAYIMPL_H
