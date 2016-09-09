#ifndef GRIDLAYOUTIMPLFACTORY_H
#define GRIDLAYOUTIMPLFACTORY_H

#include <array>

#include "gridlayout.h"
#include "gridlayoutimplyee.h"
#include <iostream>


class GridLayoutImplFactory
{
public:

    static std::unique_ptr<GridLayoutImpl>
    createGridLayoutImpl(uint32 nbDims,
                         uint32 interpOrder,
                         std::string const& layoutName,
                         std::array<uint32,3> nbrCellsXYZ,
                         std::array<double,3> dxdydz     )
    {
        if (nbDims != 1 && nbDims != 2 && nbDims != 3)
        {
             throw std::runtime_error("Error : GridLayoutImplFactory - wrong dimensionality");
        }

        if (layoutName == "yee")
        {
            return std::unique_ptr<GridLayoutImpl> (
                        new GridLayoutImplYee( nbDims, interpOrder,
                                               nbrCellsXYZ, dxdydz ));
        }
        //else if{}

        else
        {
            throw std::runtime_error("Error : GridLayoutImplFactory - unknown grid layout");
        }

    } // end factory method


};




#endif // GRIDLAYOUTIMPLFACTORY_H
