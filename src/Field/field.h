#ifndef FIELD_H
#define FIELD_H


#include <string>
#include <vector>
#include <cstddef>

#include "types.h"
#include "hybridenums.h"


class Field
{

public:

    Field(uint32 nx, uint32 ny, uint32 nz,
          HybridQuantity qtyType, std::string name):
        name_(name), qtyType_{qtyType},
        shape_{nx,ny,nz},data_{}  {data_.resize(nx*ny*nz);}

    Field(Field&& source) = default;
    Field& operator=(Field&& source) = default;

    Field(Field const& source) = default;

    // http://en.cppreference.com/w/cpp/language/copy_assignment
    // gets deleted if implicitly defined and if move construct and assign
    // are declared ... why ? (lead to error un Ohm if not defaulted).
    Field& operator=(Field const& source) = default;


    std::string name() const {return name_;}

    // operators for 1D, 2D and 3D read/write access for convenience

    double const& operator()(uint32 ix) const {return data_[ix];}
    double const& operator()(uint32 ix, uint32 iy) const { return data_[iy+shape_[1]*ix]; }
    double const& operator()(uint32 ix, uint32 iy, uint32 iz) const
    {return data_[iz + shape_[2]*iy + shape_[1]*shape_[2]*ix]; }


    double& operator()(uint32 ix) {return data_[ix];}
    double& operator()(uint32 ix, uint32 iy) { return data_[iy+shape_[1]*ix]; }
    double& operator()(uint32 ix, uint32 iy, uint32 iz)
    {return data_[iz + shape_[2]*iy + shape_[1]*shape_[2]*ix]; }

    std::vector<uint32> shape() const {return shape_;}

    HybridQuantity type() const {return qtyType_;}

private:

    std::string name_;
    HybridQuantity qtyType_ ;

    std::vector<uint32> shape_;

    std::vector<double> data_;


};

#endif // FIELD_H


