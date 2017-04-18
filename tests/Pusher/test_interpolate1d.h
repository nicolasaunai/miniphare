#ifndef TEST_INTERPOLATE1D_H
#define TEST_INTERPOLATE1D_H


#include "types.h"
#include "utilityphare.h"

#include "grid/gridlayout.h"
#include "grid/gridlayoutimplfactory.h"
#include "grid/gridlayoutimplyee.h"




struct InterpPushParams;


std::vector<InterpPushParams>  getPush1dParamsFromFile() ;





/* ----------------------------------------------------------------------------
 *
 *                      Small structure to check
 *                       the index list
 * ---------------------------------------------------------------------------- */

struct InterpPushParams
{

    double tbegin, tend ;
    uint32 nstep ;

    double q, m ;

    // Initial conditions
    double x0, y0, z0 ;
    double vx0, vy0, vz0 ;


    uint32 testId ;

    static uint32 testCaseNbr ;

    // additional attributes used to initialize
    // a GridLayout object

    std::array<double, 3> origin = { {0., 0., 0.} } ;
    std::array<double, 3> dxdydz = { {0.1, 0., 0.} } ;
    std::array<uint32, 3> nbrCells = { {1000, 0, 0} } ;

    uint32 nbDim = 1 ;

    std::string lattice = "yee" ;

    uint32 interpOrder = 1 ;

    InterpPushParams(): testId{ testCaseNbr }
     { ++testCaseNbr ; }

};


#endif // TEST_INTERPOLATE1D_H
