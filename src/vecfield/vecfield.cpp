
#include <iostream>
#include <cstddef>


#include "vecfield.h"



VecField::VecField(AllocSizeT xComponentSizes,
                   AllocSizeT yComponentSizes,
                   AllocSizeT zComponentSizes,
                   std::array<HybridQuantity, NBR_COMPO> componentTypes,
                   std::string name)
    :
            xComponent_( xComponentSizes.nx_,
                         xComponentSizes.ny_,
                         xComponentSizes.nz_,
                         componentTypes[0], name.insert(1,"_x") ),

            yComponent_( yComponentSizes.nx_,
                         yComponentSizes.ny_,
                         yComponentSizes.nz_,
                         componentTypes[1], name.insert(1,"_y") ),

            zComponent_( zComponentSizes.nx_,
                         zComponentSizes.ny_,
                         zComponentSizes.nz_,
                         componentTypes[2], name.insert(1,"_z") )
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
    std::cout << iComp << std::endl;

    switch (iComp)
    {
        case VecField::VecX:
            return xComponent_;
        break;

        case VecField::VecY:
            return yComponent_;
        break;

        case VecField::VecZ:
            return zComponent_;
        break;

        default:
            throw std::runtime_error("wrong component");
    }

}



const Field& VecField::component(uint32 iComp) const
{
    switch (iComp)
    {
        case VecField::VecX:
            return xComponent_;
        break;

        case VecField::VecY:
            return yComponent_;
        break;

        case VecField::VecZ:
            return zComponent_;
        break;

        default:
            throw std::runtime_error("wrong component");
    }


}


#if 0
void avg(Vecfield const& VF1, Vecfield const& VF2, Vecfield& Vavg)
{

}
#endif
