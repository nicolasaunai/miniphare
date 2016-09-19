#ifndef FIELD_H
#define FIELD_H


#include <string>
#include <vector>
#include <cstddef>

#include "types.h"


class Field
{

public:


    Field(uint32 nx, uint32 ny, uint32 nz, std::string name):name_(name),
        shape_{nx,ny,nz},ndims_{2},data_{}{data_.resize(nx*ny*nz);}

    Field(Field&& source) = default;
    Field& operator=(Field&& source) = default;

    Field(Field const& source) = default;
    Field& operator=(Field const& source) = default;

    void zero() {for (double& x : data_) x=0;}

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


    std::vector<double>::iterator begin() {return data_.begin();}
    std::vector<double>::const_iterator begin() const {return data_.begin();}

    std::vector<double>::iterator end() {return data_.end();}
    std::vector<double>::const_iterator end() const {return data_.end();}

    std::vector<uint32> shape() const {return shape_;}
    uint32 nbDimensions()const{return ndims_;}

private:

    std::string name_;
    std::vector<uint32> shape_;
    uint32 ndims_;
    std::vector<double> data_;


};

#endif // FIELD_H


