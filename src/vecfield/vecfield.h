
#ifndef VECFIELD_H
#define VECFIELD_H


#include <string>
#include <memory>
#include <iostream>

#include "constants.h"
#include "types.h"

#include "Field/field.h"

//namespace VectorField{

class VecField
{

private:

    std::string name_;

    Field xComponent_;
    Field yComponent_;
    Field zComponent_;


    friend bool sameShape(VecField const& v1, VecField const& v2);


public:

    static const uint32 VecX = 0;
    static const uint32 VecY = 1;
    static const uint32 VecZ = 2;


    VecField(std::array<AllocSizeT, NBR_COMPO> allComponentSizes,
             std::array<HybridQuantity, NBR_COMPO> qtyType, std::string name)
        : name_(name),
          xComponent_( allComponentSizes[0].nx_, allComponentSizes[0].ny_,
          allComponentSizes[0].nz_, qtyType[0], name.insert(1,"_x") ),
          yComponent_( allComponentSizes[1].nx_, allComponentSizes[1].ny_,
          allComponentSizes[1].nz_, qtyType[1], name.insert(1,"_y") ),
          zComponent_( allComponentSizes[2].nx_, allComponentSizes[2].ny_,
          allComponentSizes[2].nz_, qtyType[2], name.insert(1,"_z") ) {}


    // Deprecated Constructor
//    VecField(uint32 nx, uint32 ny, uint32 nz, std::string name):name_(name),
//        xComponent_(nx, ny, nz, name.insert(1,"_x")),
//        yComponent_(nx, ny, nz, name.insert(1,"_y")),
//        zComponent_(nx, ny, nz, name.insert(1,"_z")) {}


    // I don't like calling 3 times shape()
    // what's the return type of shape?
    std::vector<uint32> shape() const{return xComponent_.shape();}

    std::string name()const {return name_;}


    Field& component(uint32 iComp);

    const Field& component(uint32 iComp) const;

    void dotProduct(VecField const& v1, VecField const& v2, Field& v1dotv2);




};


//void avg(Vecfield const& VF1, Vecfield const& VF2, Vecfield& Vavg);

//}


#endif // VECFIELD_H

