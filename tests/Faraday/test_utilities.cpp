
#include <fstream>


#include "test_faraday1d.h"



HybridQuantity GetHybridQtyFromString( std::string field )
{
    HybridQuantity qty = HybridQuantity::count ;

    if( field == "Bx" )
    {
        qty = HybridQuantity::Bx ;
    } else if( field == "By" )
    {
        qty = HybridQuantity::By ;
    } else if( field == "Bz" )
    {
        qty = HybridQuantity::Bz ;
    } else if( field == "Ex" )
    {
        qty = HybridQuantity::Ex ;
    } else if( field == "Ey" )
    {
        qty = HybridQuantity::Ey ;
    } else if( field == "Ez" )
    {
        qty = HybridQuantity::Ez ;
    }

    return qty ;
}


HybridQuantity GetHybridQty(uint32 iqty)
{
    HybridQuantity quantity = HybridQuantity::count ;

    std::array<HybridQuantity, static_cast<uint32>(HybridQuantity::count) >  hybridQtyTable ;

    hybridQtyTable[0] = HybridQuantity::Bx  ;
    hybridQtyTable[1] = HybridQuantity::By  ;
    hybridQtyTable[2] = HybridQuantity::Bz  ;
    hybridQtyTable[3] = HybridQuantity::Ex  ;
    hybridQtyTable[4] = HybridQuantity::Ey  ;
    hybridQtyTable[5] = HybridQuantity::Ez  ;
    hybridQtyTable[6] = HybridQuantity::rho ;
    hybridQtyTable[7] = HybridQuantity::V   ;
    hybridQtyTable[8] = HybridQuantity::P   ;

    if( iqty < static_cast<uint32>(HybridQuantity::count) )
    {
        quantity = hybridQtyTable[iqty] ;
    } else
    {
        quantity = HybridQuantity::count ;
    }

    return quantity ;
}


std::string GetHybridQtyName(uint32 iqty)
{
    std::string qtyName{"None"} ;

    std::array<std::string, static_cast<uint32>(HybridQuantity::count) >  hybridQtyNames ;

    hybridQtyNames[0] = "Bx"  ;
    hybridQtyNames[1] = "By"  ;
    hybridQtyNames[2] = "Bz"  ;
    hybridQtyNames[3] = "Ex"  ;
    hybridQtyNames[4] = "Ey"  ;
    hybridQtyNames[5] = "Ez"  ;
    hybridQtyNames[6] = "rho" ;
    hybridQtyNames[7] = "V"   ;
    hybridQtyNames[8] = "P"   ;

    if( iqty < static_cast<uint32>(HybridQuantity::count) )
    {
        qtyName = hybridQtyNames[iqty] ;
    }

    return qtyName ;
}



std::vector<FaradayParams> getFaraday1DInputsFromFile()
{

    std::string filename{"../Faraday/faraday1D_summary.txt"};

    std::ifstream ifs1{filename};
    if (!ifs1 )
    {
        std::cout << "Could not open file : " << filename
                  << std::endl ;
        exit(-1);
    }

    uint32 nbrTestCases = 0 ;
    ifs1 >> nbrTestCases ;

    std::vector<FaradayParams> params(nbrTestCases);

    // reading parameters relative to the test cases
    for (uint32 i=0 ; i < nbrTestCases ; ++i)
    {
        ifs1 >> params[i].interpOrder ;
        ifs1 >> params[i].nbDim ;
        ifs1 >> params[i].nbrCells[0] ;
        ifs1 >> params[i].nbrCells[1] ;
        ifs1 >> params[i].nbrCells[2] ;
        ifs1 >> params[i].dxdydz[0] ;
        ifs1 >> params[i].dxdydz[1] ;
        ifs1 >> params[i].dxdydz[2] ;
        ifs1 >> params[i].dt ;
        ifs1 >> params[i].tStart ;
        ifs1 >> params[i].tEnd   ;
        ifs1 >> params[i].nbrTimeSteps ;
        ifs1 >> params[i].testName ;
        ifs1 >> params[i].nbrOfFields ;

        params[i].fieldNames.assign( params[i].nbrOfFields, "none") ;

        for(uint32 ifield=0 ; ifield < params[i].nbrOfFields ; ++ifield)
        {
            ifs1 >> params[i].fieldNames[ifield] ;
        }
    }

    return params ;
}


