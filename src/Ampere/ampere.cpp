
#include "ampere.h"
#include "amperefactory.h"
#include "grid/gridlayout.h"

/**
 * @brief Faraday::Faraday builds a Faraday object from the time step and the grid layout
 * @param dt : time step
 * @param layout : GridLayout
 */
Ampere::Ampere(double dt, GridLayout const& layout)
    : implPtr_{AmpereImplFactory::createAmpereImpl(dt, layout)}
{
}




void Ampere::operator()(VecField const& B, VecField& Jnew)
{
    return (*implPtr_)(B, Jnew);
}

