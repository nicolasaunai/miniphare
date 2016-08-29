#ifndef GRIDLAYOUTIMPLFACTORY_H
#define GRIDLAYOUTIMPLFACTORY_H

#include <array>

#include "gridlayout.h"
#include "gridlayoutimplyee.h"


class GridLayoutImplFactory
{
public:

    static std::unique_ptr<GridLayoutImpl> createGridLayoutImpl(uint32 nbDims)
    {
        std::string layoutName = "yee";

        if (layoutName == "yee")
        {
            return std::unique_ptr<GridLayoutImpl> ( new GridLayoutImplYee(nbDims));
        }
        //else if{}

        else
        {
            throw std::runtime_error("Error : GridLayoutImplFactory - unknown grid layout");
        }

    } // end factory method


};




#endif // GRIDLAYOUTIMPLFACTORY_H
