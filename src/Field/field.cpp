#include "field.h"

Field::Field(const AllocSizeT &allocSize, HybridQuantity qtyType, std::string name)

    : name_(name), qtyType_{qtyType}, shape_{allocSize.nx_,allocSize.ny_,allocSize.nz_}, data_{}
{
    data_.resize(shape_[0] * shape_[1] * shape_[2]);
}
