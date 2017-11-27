
#include "ampere.h"
#include "amperefactory.h"
#include "data/grid/gridlayout.h"

/**
 * @brief Faraday::Faraday builds a Faraday object from the time step and the grid layout
 * @param dt : time step
 * @param layout : GridLayout
 */
Ampere::Ampere(GridLayout const& layout)
    : implPtr_{AmpereImplFactory::createAmpereImpl(layout)}
{
}




void Ampere::operator()(VecField const& B, VecField& Jnew) const
{
    return (*implPtr_)(B, Jnew);
}
