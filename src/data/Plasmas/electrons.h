#ifndef ELECTRONS_H
#define ELECTRONS_H


#include "data/grid/gridlayout.h"
#include "data/vecfield/vecfield.h"



/**
 * @brief private abstract implementation of the electrons
 * can compute 1D, 2D or 3D electron properties.
 */
class ElectronsImpl
{
protected:
    VecField Ve_;
    Field Pe_;
    double Te_;
    GridLayout layout_;

public:
    ElectronsImpl(GridLayout const& layout, double Te)
        : Ve_{layout.allocSize(HybridQuantity::V),
              layout.allocSize(HybridQuantity::V),
              layout.allocSize(HybridQuantity::V),
              {{HybridQuantity::V, HybridQuantity::V, HybridQuantity::V}},
              "_electronBulkFlow"}
        , Pe_{layout.allocSize(HybridQuantity::P), HybridQuantity::P, "_electronPressure"}
        , Te_{Te}
        , layout_{layout}
    {
    }


    virtual VecField const& bulkVel(VecField const& Vi, Field const& Ni, VecField const& J) = 0;

    virtual Field const& pressure(Field const& Ni) = 0;
};




class Electrons
{
private:
    std::unique_ptr<ElectronsImpl> Impl_;


public:
    Electrons(GridLayout const& layout, double Te);

    VecField const& bulkVel(VecField const& Vi, Field const& Ni, VecField const& J)
    {
        return Impl_->bulkVel(Vi, Ni, J);
    }

    Field const& pressure(Field const& Ni) { return Impl_->pressure(Ni); }
};

#endif // ELECTRONS_H
