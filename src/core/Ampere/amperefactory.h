#ifndef AMPEREFACTORY_H
#define AMPEREFACTORY_H


#include <memory>
#include <vector>

#include "ampere.h"
#include "ampereimpl1d.h"
#include "data/grid/gridlayout.h"
#include "utilities/types.h"



class AmpereImplFactory
{
public:
    static std::unique_ptr<AmpereImpl> createAmpereImpl(GridLayout const& layout)
    {
        std::unique_ptr<AmpereImpl> ampere;

        switch (layout.nbDimensions())
        {
            case 1: ampere = std::unique_ptr<AmpereImpl>(new AmpereImpl1D(layout)); break;

            default: throw std::runtime_error("Error : FaradayFactory - dimension must be 1D");
        }

        return ampere;
    }
};



#endif // AMPEREFACTORY_H
