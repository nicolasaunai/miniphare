
#include <cstddef>
#include <iostream>


#include "vecfield.h"



VecField::VecField(AllocSizeT xComponentSizes, AllocSizeT yComponentSizes,
                   AllocSizeT zComponentSizes, std::array<HybridQuantity, NBR_COMPO> componentTypes,
                   std::string name)
    : name_{name}
    , xComponent_(xComponentSizes, componentTypes[0], name + "_x")
    , yComponent_(yComponentSizes, componentTypes[1], name + "_y")
    , zComponent_(zComponentSizes, componentTypes[2], name + "_z")
{
}



#if 0
bool sameShape(const VecField &v1, const VecField &v2)
{
    return true; //TODO should be implemented
}

#endif




Field& VecField::component(uint32 iComp)
{
    //    std::cout << iComp << std::endl;

    switch (iComp)
    {
        case VecField::VecX: return xComponent_; break;

        case VecField::VecY: return yComponent_; break;

        case VecField::VecZ: return zComponent_; break;

        default: throw std::runtime_error("wrong component");
    }
}



const Field& VecField::component(uint32 iComp) const
{
    switch (iComp)
    {
        case VecField::VecX: return xComponent_; break;

        case VecField::VecY: return yComponent_; break;

        case VecField::VecZ: return zComponent_; break;

        default: throw std::runtime_error("wrong component");
    }
}



std::vector<std::reference_wrapper<Field>> VecField::components()
{
    return {xComponent_, yComponent_, zComponent_};
}


std::vector<std::reference_wrapper<Field const>> VecField::components() const
{
    return {xComponent_, yComponent_, zComponent_};
}
