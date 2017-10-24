#ifndef ELECTRONSIMPL1D_H
#define ELECTRONSIMPL1D_H

#include "data/grid/gridlayout.h"
#include "electrons.h"



class ElectronsImpl1D : public ElectronsImpl
{
public:
    ElectronsImpl1D(GridLayout const& layout, double Te)
        : ElectronsImpl(layout, Te)
    {
    }

    virtual VecField const& bulkVel(VecField const& Vi, Field const& Ni, VecField const& J) final;

    virtual Field const& pressure(Field const& Ni) final;
};

#endif // ELECTRONSIMPL1D_H
