#ifndef TEST_WEIGHTS_H
#define TEST_WEIGHTS_H

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "types.h"
#include "utilityphare.h"

#include "grid/gridlayout.h"
#include "grid/gridlayoutimplfactory.h"
#include "grid/gridlayoutimplyee.h"

#include "Field/field.h"


struct WeightParams;

HybridQuantity GetHybridQtyFromString(std::string field);

HybridQuantity GetHybridQty(uint32 iqty);

std::string GetHybridQtyName(uint iqty);




/* ----------------------------------------------------------------------------
 *
 *                   Small structure to check
 *                   the ponderations deposited
 *                   during projection
 *
 * ---------------------------------------------------------------------------- */

struct WeightParams
{
    uint32 interpOrder;
    uint32 nbrX;

    double dx;

    std::string field;

    double xmin;
    double xpart;

    // additional attributes used to initialize
    // a GridLayout object

    std::array<double, 3> dxdydz;
    std::array<uint32, 3> nbrCells;

    uint32 nbDim = 1;

    std::string lattice = "yee";

    WeightParams(uint32 order, uint32 nbrCellX, double dx, std::string field, double xmin,
                 double xpart)
        : interpOrder{order}
        , nbrX{nbrCellX}
        , dx{dx}
        , field{field}
        , xmin{xmin}
        , xpart{xpart}
        , dxdydz{{dx, 0., 0.}}
        , nbrCells{{nbrX, 0, 0}}
    {
    }

    WeightParams()
        : dxdydz{{0., 0., 0.}}
        , nbrCells{{0, 0, 0}}
    {
    }
};


#endif // TEST_WEIGHTS_H
