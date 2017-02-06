
#ifndef VECFIELD_H
#define VECFIELD_H


#include <string>
#include <memory>
#include <iostream>
#include <array>
#include <vector>
#include <functional>

#include "constants.h"
#include "types.h"

#include "Field/field.h"



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


    VecField(AllocSizeT xComponent,
             AllocSizeT yComponent,
             AllocSizeT zComponent,
             std::array<HybridQuantity, NBR_COMPO> componentTypes,
             std::string name);


    // Deprecated Constructor
//    VecField(uint32 nx, uint32 ny, uint32 nz, std::string name):name_(name),
//        xComponent_(nx, ny, nz, name.insert(1,"_x")),
//        yComponent_(nx, ny, nz, name.insert(1,"_y")),
//        zComponent_(nx, ny, nz, name.insert(1,"_z")) {}


    // I don't like calling 3 times shape()
    // what's the return type of shape?
    std::vector<uint32> shape() const{return xComponent_.shape();}

    std::string name()const {return name_;}

    void zero() {xComponent_.zero(); yComponent_.zero(); zComponent_.zero();}

    Field& component(uint32 iComp);
    const Field& component(uint32 iComp) const;

    void dotProduct(VecField const& v1, VecField const& v2, Field& v1dotv2);

    std::vector<std::reference_wrapper<Field>> components();



};


//void avg(Vecfield const& VF1, Vecfield const& VF2, Vecfield& Vavg);




#endif // VECFIELD_H

