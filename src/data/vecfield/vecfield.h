
#ifndef VECFIELD_H
#define VECFIELD_H


#include <array>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "utilities/constants.h"
#include "utilities/types.h"

#include "data/Field/field.h"



class VecField
{
private:
    std::string name_;

    Field xComponent_;
    Field yComponent_;
    Field zComponent_;


    // friend bool sameShape(VecField const& v1, VecField const& v2);


public:
    static const uint32 VecX = 0;
    static const uint32 VecY = 1;
    static const uint32 VecZ = 2;

    VecField() = default;

    VecField(AllocSizeT xComponent, AllocSizeT yComponent, AllocSizeT zComponent,
             std::array<HybridQuantity, NBR_COMPO> componentTypes, std::string name);


    // I don't like calling 3 times shape()
    // what's the return type of shape?
    std::vector<uint32> shape() const { return xComponent_.shape(); }

    std::string name() const { return name_; }

    void zero()
    {
        xComponent_.zero();
        yComponent_.zero();
        zComponent_.zero();
    }

    Field& component(uint32 iComp);
    const Field& component(uint32 iComp) const;

    void dotProduct(VecField const& v1, VecField const& v2, Field& v1dotv2);

    std::vector<std::reference_wrapper<Field>> components();
    std::vector<std::reference_wrapper<Field const>> components() const;
};



#endif // VECFIELD_H
