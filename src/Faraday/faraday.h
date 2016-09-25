#ifndef FARADAY_H
#define FARADAY_H

#include <memory>

#include "faradayimpl.h"

#include "vecfield/vecfield.h"
#include "Field/field.h"
#include "grid/gridlayout.h"



/**
 * @brief The Faraday class represents the faraday equation solver.
 *
 * Faraday provides client code with an operator() taking the old magnetic field,
 * the electric field and a reference to the updated magnetic field vector.
 * The class encapsulates a unique pointer to an abstract implementation of the
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
