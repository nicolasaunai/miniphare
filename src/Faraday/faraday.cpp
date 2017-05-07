
#include "faraday.h"
#include "faradayfactory.h"
#include "grid/gridlayout.h"

/**
 * @brief Faraday::Faraday builds a Faraday object from the time step and the grid layout
 * @param dt : time step
 * @param layout : GridLayout
 */
Faraday::Faraday(double dt, GridLayout const& layout)
    : implPtr_{FaradayImplFactory::createFaradayImpl(dt, layout)}
{
}




void Faraday::operator()(VecField const& E, VecField const& B, VecField& Bnew)
{
    return (*implPtr_)(E, B, Bnew);
}
