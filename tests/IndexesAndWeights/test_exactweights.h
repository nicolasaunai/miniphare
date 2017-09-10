#ifndef TEST_EXACTWEIGHTS_H
#define TEST_EXACTWEIGHTS_H

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "types.h"
#include "utilities/box.h"

#include "grid/gridlayout.h"
#include "grid/gridlayoutimplfactory.h"
#include "grid/gridlayoutimplyee.h"

#include "Field/field.h"


struct ExactWeightsParams;

HybridQuantity GetHybridQtyFromString(std::string field);

HybridQuantity GetHybridQty(uint32 iqty);

std::string GetHybridQtyName(uint iqty);

std::vector<ExactWeightsParams> getExactWeightsParamsFromFile();




/* ----------------------------------------------------------------------------
 *
 *                      Small structure to check
 *                       the index list
 * ---------------------------------------------------------------------------- */

struct ExactWeightsParams
{
    uint32 interpOrder;
    uint32 nbrX;

    double dx;

    std::string field;

    double xmin;
    double xpart;

    uint32 testId;

    static uint32 testCaseNbr;

    // additional attributes used to initialize
    // a GridLayout object

    std::array<double, 3> dxdydz;
    std::array<uint32, 3> nbrCells;

    uint32 nbDim = 1;

    std::string lattice = "yee";

    Point origin{0., 0., 0.};

    ExactWeightsParams()
        : testId{testCaseNbr}
        , dxdydz{{0., 0., 0.}}
        , nbrCells{{0, 0, 0}}
    {
        ++testCaseNbr;
    }
};

#endif // TEST_EXACTWEIGHTS_H
