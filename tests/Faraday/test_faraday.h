#ifndef TEST_FARADAY_H
#define TEST_FARADAY_H


#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "types.h"
#include "utility.h"

#include "grid/gridlayout.h"
#include "grid/gridlayoutimplfactory.h"
#include "grid/gridlayoutimplyee.h"

#include "Field/field.h"


struct GridLayoutParams;

HybridQuantity GetHybridQty(uint iqty) ;

std::string GetHybridQtyName(uint iqty) ;

std::vector<GridLayoutParams> getFaraday1DInputsFromFile() ;



#define  MAX_SIZE 1000




/* ----------------------------------------------------------------------------
 *
 *                            GridLayout TEST
 * ---------------------------------------------------------------------------- */

struct GridLayoutParams
{
    uint32 interpOrder;
    uint32 nbDim;

    HybridQuantity qty;
    std::string qtyName;
    uint32 iqty ;        // integer equals to static_cast<uint32> (qty)

    std::string functionName ;

    uint32 icase ;

    std::string layoutName ;

    std::array<uint32, 3> nbrCells;
    std::array<double, 3> dxdydz;

    std::array<uint32,3> allocSizes;
    std::array<uint32,3> allocSizeDerived;

    std::array<uint32,3> PSI;
    std::array<uint32,3> PEI;
    std::array<uint32,3> GSI;
    std::array<uint32,3> GEI;

    uint32  field_iStart ;
    uint32  field_iEnd   ;
    Point origin{0., 0., 0.} ;

    std::vector<double>  fieldXCoords ;
    std::vector<double>  fieldXValues ;

    std::vector<double>  derivedFieldXCoords ;
    std::vector<double>  derivedFieldXValues ;

    std::vector<double>  cellCenteredXCoords ;

    std::string testComment;

    GridLayoutParams() = default ;

    GridLayoutParams(double dx, double dy, double dz,
                     uint32 nbrCellx, uint32 nbrCelly, uint32 nbrCellz,
                     std::string const& name,
                     uint32 dim, std::string comment):
        nbDim{dim}, layoutName{name},
        nbrCells{ {nbrCellx,nbrCelly,nbrCellz} }, dxdydz{ {dx,dy,dz} },
        testComment{comment}
    {

    }

};


#endif // TEST_FARADAY_H