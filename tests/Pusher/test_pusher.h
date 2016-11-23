#ifndef TEST_INDEXES_H
#define TEST_INDEXES_H


#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "types.h"
#include "utility.h"

#include "grid/gridlayout.h"
#include "grid/gridlayoutimplfactory.h"
#include "grid/gridlayoutimplyee.h"

#include "Field/field.h"


struct Pusher1dParams;

HybridQuantity GetHybridQtyFromString( std::string field ) ;

HybridQuantity GetHybridQty(uint32 iqty) ;

std::string GetHybridQtyName(uint iqty) ;

std::vector<Pusher1dParams>  getPusher1dParamsFromFile() ;





/* ----------------------------------------------------------------------------
 *
 *                      Small structure to check
 *                       the index list
 * ---------------------------------------------------------------------------- */

struct Pusher1dParams
{

    uint32 interpOrder ;
    uint32 nbrX ;

    double dx ;

    std::string field ;

    double xmin ;
    double xpart ;

    uint32 testId ;

    static uint32 testCaseNbr ;

    // additional attributes used to initialize
    // a GridLayout object

    std::array<double, 3> dxdydz ;
    std::array<uint32, 3> nbrCells ;

    uint32 nbDim = 1 ;

    std::string lattice = "yee" ;

    Pusher1dParams(): testId{ testCaseNbr },
        dxdydz{ {0., 0., 0.} }, nbrCells{ {0, 0, 0} }
    { ++testCaseNbr ;  }

};


#endif // TEST_INDEXES_H
