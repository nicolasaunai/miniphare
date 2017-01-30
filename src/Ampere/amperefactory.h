#ifndef AMPEREFACTORY_H
#define AMPEREFACTORY_H


#include <memory>
#include <vector>

#include "Ampere/ampere.h"
#include "ampereimpl1d.h"
#include "types.h"
#include "grid/gridlayout.h"



class AmpereImplFactory
{
public:
    static std::unique_ptr<AmpereImpl>
    createAmpereImpl(double dt, GridLayout const& layout)
    {
        std::unique_ptr<AmpereImpl> ampere ;

        switch (layout.nbDimensions())
        {
        case 1:
            ampere = std::unique_ptr<AmpereImpl> ( new AmpereImpl1D(dt, layout));
            break;

        default:
            throw std::runtime_error("Error : FaradayFactory - dimension must be 1D");
        }

        return ampere ;
    }
};



#endif // AMPEREFACTORY_H
