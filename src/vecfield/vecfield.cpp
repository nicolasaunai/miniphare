
#include <iostream>
#include <cstddef>


#include "vecfield.h"



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
