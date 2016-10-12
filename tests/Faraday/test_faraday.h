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


struct FaradayParams;

HybridQuantity GetHybridQtyFromString( std::string field ) ;

std::string GetHybridQtyName(uint iqty) ;

std::vector<FaradayParams> getFaraday1DInputsFromFile() ;



#define  MAX_SIZE 1000




/* ----------------------------------------------------------------------------
 *
 *                            GridLayout TEST
 * ---------------------------------------------------------------------------- */

struct FaradayParams
{
    uint32 interpOrder;

    uint32 nbDim ;

//    HybridQuantity qty;
//    std::string qtyName;
//    uint32 iqty ;

    std::array<uint32, 3> nbrCells;
    std::array<double, 3> dxdydz;

//    Point origin{0., 0., 0.} ;

    double dt ;
    double tStart, tEnd ;

    std::string testName ;

    uint32 nbrOfFields ;

    std::vector<std::string> fieldNames ;

    uint32 nbrTimeSteps ;

    std::vector<double>  x_Bx, y_Bx, z_Bx ;
    std::vector<double>  Bx ;

    std::vector<double>  x_By, y_By, z_By ;
    std::vector<double>  By ;

    std::vector<double>  x_Bz, y_Bz, z_Bz ;
    std::vector<double>  Bz ;

    std::vector<double>  x_Ex, y_Ex, z_Ex ;
    std::vector<double>  Ex ;

    std::vector<double>  x_Ey, y_Ey, z_Ey ;
    std::vector<double>  Ey ;

    std::vector<double>  x_Ez, y_Ez, z_Ez ;
    std::vector<double>  Ez ;


    std::string testComment;

    FaradayParams() = default ;

    FaradayParams(double dx, double dy, double dz,
                  uint32 nbrCellx, uint32 nbrCelly, uint32 nbrCellz,
                  std::string comment):
        nbrCells{ {nbrCellx,nbrCelly,nbrCellz} }, dxdydz{ {dx,dy,dz} },
        testComment{comment}
    {

    }

};


#endif // TEST_FARADAY_H
