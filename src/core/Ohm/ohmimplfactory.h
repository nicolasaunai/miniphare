#ifndef OHMIMPLFACTORY_H
#define OHMIMPLFACTORY_H

#include "data/grid/gridlayout.h"
#include "ohm.h"
#include "ohmimpl1d.h"
#include "utilities/types.h"


class OhmImplFactory
{
public:
    static std::unique_ptr<OhmImpl> createOhmImpl(GridLayout const& layout)
    {
        switch (layout.nbDimensions())
        {
            case 1: return std::unique_ptr<OhmImpl>(new OhmImpl1D(layout, 1e-3, 1e-4));

#if 0
            case 2:
                std::cout << "not yet implemented\n";
                break;
#endif

            default:
                throw std::runtime_error(
                    "Error : FaradayFactory - dimension must be either 1D, 2D or 3D");
        }
    }
};




#endif // OHMIMPLFACTORY_H
