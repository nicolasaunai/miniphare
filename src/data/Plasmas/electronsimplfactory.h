#ifndef ELECTRONSIMPLFACTORY_H
#define ELECTRONSIMPLFACTORY_H


#include "electrons.h"
#include "electronsimpl1d.h"
#include "utilities/types.h"

class ElectronsImplFactory
{
public:
    static std::unique_ptr<ElectronsImpl> createElectronImpl(GridLayout const& layout, double Te)
    {
        switch (layout.nbDimensions())
        {
            case 1: return std::unique_ptr<ElectronsImpl>{new ElectronsImpl1D{layout, Te}};


            case 2:
            case 3:
            default: throw std::runtime_error("Not implemented");
        }
    }
};


#endif // ELECTRONSIMPLFACTORY_H
