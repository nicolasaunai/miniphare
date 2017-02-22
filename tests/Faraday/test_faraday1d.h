#ifndef TEST_FARADAY_H
#define TEST_FARADAY_H


#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "types.h"
#include "utilityphare.h"

#include "grid/gridlayout.h"
#include "grid/gridlayoutimplfactory.h"
#include "grid/gridlayoutimplyee.h"

#include "Field/field.h"


struct FaradayParams;

HybridQuantity GetHybridQtyFromString( std::string field ) ;

HybridQuantity GetHybridQty(uint32 iqty) ;

std::string GetHybridQtyName(uint iqty) ;

std::vector<FaradayParams> getFaraday1DInputsFromFile() ;



#define  MAX_SIZE 1000




/* ----------------------------------------------------------------------------
 *
 *                            Faraday TEST
 * ---------------------------------------------------------------------------- */

struct FaradayParams
{
    uint32 interpOrder;

    uint32 nbDim ;

    std::array<uint32, 3> nbrCells;
    std::array<double, 3> dxdydz;

    double dt ;
    double tStart, tEnd ;

    std::string testName ;

    uint32 nbrOfFields ;

    std::vector<std::string> fieldNames ;

    uint32 nbrTimeSteps ;

    // we have 6 components for the electromag field in the most complex case
    // Bx, By, Bz, Ex, Ey, Ez
    //    std::array<SuperVectorT, 6> fieldInputs ;
    std::array<Field, 6> fieldInputs ;

    std::string testComment;

    FaradayParams():fieldInputs \
         { { Field( Field(AllocSizeT(10,1,1),HybridQuantity::count,"none") ),
             Field( Field(AllocSizeT(10,1,1),HybridQuantity::count,"none") ),
             Field( Field(AllocSizeT(10,1,1),HybridQuantity::count,"none") ),
             Field( Field(AllocSizeT(10,1,1),HybridQuantity::count,"none") ),
             Field( Field(AllocSizeT(10,1,1),HybridQuantity::count,"none") ),
             Field( Field(AllocSizeT(10,1,1),HybridQuantity::count,"none") )} }
    {
    }

    FaradayParams(double dx, double dy, double dz,
                  uint32 nbrCellx, uint32 nbrCelly, uint32 nbrCellz,
                  std::string comment):
        nbrCells{ {nbrCellx,nbrCelly,nbrCellz} }, dxdydz{ {dx,dy,dz} },
        fieldInputs{ {Field( Field(AllocSizeT(10,1,1),HybridQuantity::count,"none") ),
                      Field( Field(AllocSizeT(10,1,1),HybridQuantity::count,"none") ),
                      Field( Field(AllocSizeT(10,1,1),HybridQuantity::count,"none") ),
                      Field( Field(AllocSizeT(10,1,1),HybridQuantity::count,"none") ),
                      Field( Field(AllocSizeT(10,1,1),HybridQuantity::count,"none") ),
                      Field( Field(AllocSizeT(10,1,1),HybridQuantity::count,"none") )} },
        testComment{comment}
    {

    }

};


#endif // TEST_FARADAY_H
