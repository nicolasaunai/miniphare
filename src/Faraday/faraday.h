#ifndef FARADAY_H
#define FARADAY_H

#include <memory>


#include "vecfield/vecfield.h"
#include "Field/field.h"
#include "grid/gridlayout.h"




/**
 * @brief Interface for concrete Faraday implementations (1D, 2D, 3D)
 */
class FaradayImpl
{
    public:
        virtual void operator()(VecField const& E, VecField const& B, VecField& Bnew) = 0;
        virtual ~FaradayImpl();
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
    FaradayImplInternals(double dt, GridLayout const& layout):dt_{dt},layout_{layout}{}

protected:
    double dt_;
    GridLayout layout_;
};





/**
 * @brief The Faraday class represents the faraday equation solver.
 *
 * Faraday provides client code with an operator() taking the old magnetic field,
 * the electric field and a reference to the updated magnetic field vector.
 * The class encapsulate a unique pointer to an abstract implementation of the
 * solver, which is realized by concrete implementations knowing the dimensionality etc.
 *
 * Faraday is intended to be part of a Solver object and is therefore move-only.
 */
class Faraday
{
private:

    std::unique_ptr<FaradayImpl> implPtr_;


public:


    explicit Faraday(double dt, GridLayout const& layout);

    // Faraday is a move-only object.
    Faraday(Faraday const& source) = delete;
    Faraday& operator=(Faraday const& source) = delete;

    Faraday(Faraday&& source)            = default;
    Faraday& operator=(Faraday&& source) = default;



    void operator()(VecField const& E,
                    VecField const& B,
                    VecField& Bnew);

};



#endif // FARADAY_H
