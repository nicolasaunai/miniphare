#ifndef AMPERE_H
#define AMPERE_H

#include <memory>

#include "ampereimpl.h"
#include "data/grid/gridlayout.h"
#include "data/vecfield/vecfield.h"



/**
 * @brief The Ampere class is used to compute the current J by means of a
 * truncated Ampere's equation
 *
 * Ampere provides client code with an operator() taking a magnetic field
 * and the updated current field vector.
 * The class encapsulates a unique pointer to an abstract implementation,
 * which is realized by concrete implementations knowing the dimensionality etc.
 *
 * Ampere is intended to be part of a Solver object and is therefore move-only.
 */
class Ampere
{
private:
    std::unique_ptr<AmpereImpl> implPtr_;


public:
    explicit Ampere(GridLayout const& layout);

    // Ampere is a move-only object.
    Ampere(Ampere const& source) = delete;
    Ampere& operator=(Ampere const& source) = delete;

    Ampere(Ampere&& source) = default;
    Ampere& operator=(Ampere&& source) = default;

    ~Ampere() = default;

    void operator()(VecField const& B, VecField& Jnew) const;
};



#endif // AMPERE_H
