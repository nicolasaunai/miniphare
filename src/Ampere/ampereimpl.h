#ifndef AMPEREIMPL_H
#define AMPEREIMPL_H

#include "vecfield/vecfield.h"
#include "Field/field.h"
#include "grid/gridlayout.h"




/**
 * @brief Interface for concrete Ampere implementations (1D, 2D, 3D)
 */
class AmpereImpl
{
public:
    virtual ~AmpereImpl() = default ;

    virtual void operator()(VecField const& B, VecField& Jnew) = 0;
};





/**
 * @brief The AmpereImplInternals class factorizes details of implementation
 *  for the concrete derived classes inheriting from AmpereImpl.
 *
 *  The class encapsulates the time step and the GridLayout
 */
class AmpereImplInternals
{
public:
    AmpereImplInternals(GridLayout const& layout):layout_{layout}{}

protected:
    GridLayout layout_;
};


#endif // AMPEREIMPL_H
