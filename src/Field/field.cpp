#include "field.h"

Field::Field(const AllocSizeT &allocSize, HybridQuantity qtyType, std::string name)

    : name_(name)
    , qtyType_{qtyType}
    , shape_{allocSize.nx_, allocSize.ny_, allocSize.nz_}
    , data_{}
    , size_{shape_[0] * shape_[1] * shape_[2]}
{
    data_.resize(size_);
    data_.assign(size_, 0.);
}
