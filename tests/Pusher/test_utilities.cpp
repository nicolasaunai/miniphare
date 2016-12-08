
#include <fstream>

#include "test_pusher.h"
#include "test_interpolate1d.h"



std::vector<PusherParams> getPusherParamsFromFile()
{

    std::string filename{"../Pusher/odepush_summary.txt"};

    std::ifstream ifs1{filename};
    if (!ifs1 )
    {
        std::cout << "Could not open file : " << filename
                  << std::endl ;
        exit(-1);
    }

    uint32 nbrTestCases = 0 ;
    ifs1 >> nbrTestCases ;

    std::vector<PusherParams> params(nbrTestCases);

    // reading parameters relative to the test cases
    for (uint32 i=0 ; i < nbrTestCases ; ++i)
    {
        ifs1 >> params[i].tbegin ;
        ifs1 >> params[i].tend ;
        ifs1 >> params[i].nstep ;
        ifs1 >> params[i].q ;
        ifs1 >> params[i].m ;
        ifs1 >> params[i].x0 ;
        ifs1 >> params[i].y0 ;
        ifs1 >> params[i].z0 ;
        ifs1 >> params[i].vx0 ;
        ifs1 >> params[i].vy0 ;
        ifs1 >> params[i].vz0 ;
    }

    return params ;
}




std::vector<InterpPushParams> getInterpPushParamsFromFile()
{

    std::string filename{"../Pusher/odepush_summary.txt"};

    std::ifstream ifs1{filename};
    if (!ifs1 )
    {
        std::cout << "Could not open file : " << filename
                  << std::endl ;
        exit(-1);
    }

    uint32 nbrTestCases = 0 ;
    ifs1 >> nbrTestCases ;

    std::vector<InterpPushParams> params(nbrTestCases);

    // reading parameters relative to the test cases
    for (uint32 i=0 ; i < nbrTestCases ; ++i)
    {
        ifs1 >> params[i].tbegin ;
        ifs1 >> params[i].tend ;
        ifs1 >> params[i].nstep ;
        ifs1 >> params[i].q ;
        ifs1 >> params[i].m ;
        ifs1 >> params[i].x0 ;
        ifs1 >> params[i].y0 ;
        ifs1 >> params[i].z0 ;
        ifs1 >> params[i].vx0 ;
        ifs1 >> params[i].vy0 ;
        ifs1 >> params[i].vz0 ;
    }

    return params ;
}


