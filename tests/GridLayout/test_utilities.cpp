
#include <fstream>

#include "test_gridlayout.h"


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



std::vector<GridLayoutParams> getInputsFromFile()
{

    std::ifstream ifs{"../GridLayout/result.txt"};
    if (!ifs )
        exit(-1);

    uint32 orderMax = 4 ;
    uint32 dimMax = 3 ;

    uint32 nbrTestCases = orderMax * dimMax * static_cast<uint32>(HybridQuantity::count) ;

    std::vector<GridLayoutParams> params(nbrTestCases);
    uint32 iqty;

    for (uint32 i=0 ; i < nbrTestCases ; ++i)
    {
        ifs >> params[i].interpOrder >> params[i].nbDim >> iqty
        >> params[i].nbrCells[0] >>  params[i].nbrCells[1]  >>  params[i].nbrCells[2]
        >> params[i].dxdydz[0] >>  params[i].dxdydz[1]  >>  params[i].dxdydz[2]
        >> params[i].PSI[0] >> params[i].PEI[0] >> params[i].GSI[0] >> params[i].GEI[0]
        >> params[i].PSI[1] >> params[i].PEI[1] >> params[i].GSI[1] >> params[i].GEI[1]
        >> params[i].PSI[2] >> params[i].PEI[2] >> params[i].GSI[2] >> params[i].GEI[2];


        params[i].qty = GetHybridQty(iqty);
        params[i].iqty = iqty;
    }

    return params;
}



std::vector<GridLayoutParams> getAllocInputsFromFile()
{

    std::ifstream ifs{"../GridLayout/allocSizes.txt"};
    if (!ifs )
        exit(-1);

    uint32 orderMax = 4 ;
    uint32 dimMax = 3 ;

    uint32 nbrTestCases = orderMax * dimMax * static_cast<uint32>(HybridQuantity::count) ;

    std::vector<GridLayoutParams> params(nbrTestCases);
    uint32 iqty;

    for (uint32 i=0 ; i < nbrTestCases ; ++i)
    {
        ifs >> params[i].interpOrder >> params[i].nbDim >> iqty
        >> params[i].nbrCells[0] >>  params[i].nbrCells[1]  >>  params[i].nbrCells[2]
        >> params[i].dxdydz[0] >>  params[i].dxdydz[1]  >>  params[i].dxdydz[2]
        >> params[i].allocSizes[0]
        >> params[i].allocSizes[1]
        >> params[i].allocSizes[2]
        >> params[i].allocSizeDerived[0]
        >> params[i].allocSizeDerived[1]
        >> params[i].allocSizeDerived[2] ;

        params[i].qty = GetHybridQty(iqty);
        params[i].iqty = iqty;
    }

    return params;
}



