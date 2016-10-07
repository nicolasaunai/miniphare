
#include <fstream>


#include "test_faraday.h"


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



std::vector<GridLayoutParams> getFaraday1DInputsFromFile()
{

    std::string filename{"../GridLayout/deriv1D_summary.txt"};

    std::ifstream infile{filename};
    if (!infile )
    {
        std::cout << "Could not open file : " << filename
                  << std::endl ;
        exit(-1);
    }

//    uint32 orderMax = 4 ;

    uint32 numberTestFunctions = 4 ;

    //static_cast<uint32>(HybridQuantity::count)
    uint32 nbrTestCases = 1 * numberTestFunctions ;

    std::vector<GridLayoutParams> params(nbrTestCases);

    // reading parameters relative to the test cases
    for (uint32 i=0 ; i < nbrTestCases ; ++i)
    {
        uint32 iqty;

        infile >> params[i].interpOrder
               >> params[i].nbDim
               >> iqty
               >> params[i].nbrCells[params[i].nbDim]
               >> params[i].dxdydz[params[i].nbDim]
               >> params[i].field_iStart
               >> params[i].field_iEnd
               >> params[i].origin.x_
               >> params[i].origin.y_
               >> params[i].origin.z_
               >> params[i].functionName ;

        params[i].qty = GetHybridQty(iqty);
        params[i].qtyName = GetHybridQtyName(iqty);
        params[i].iqty = iqty;

        params[i].fieldXCoords.assign(MAX_SIZE, 0.) ;
        params[i].fieldXValues.assign(MAX_SIZE, 0.) ;
        params[i].derivedFieldXCoords.assign(MAX_SIZE, 0.) ;
        params[i].derivedFieldXValues.assign(MAX_SIZE, 0.) ;
    }

    return params ;
}


