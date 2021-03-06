
#include <fstream>


#include "test_gridlayout.h"


HybridQuantity GetHybridQty(uint32 iqty)
{
    HybridQuantity quantity = HybridQuantity::count;

    std::array<HybridQuantity, static_cast<uint32>(HybridQuantity::count)> hybridQtyTable;

    hybridQtyTable[0]  = HybridQuantity::Bx;
    hybridQtyTable[1]  = HybridQuantity::By;
    hybridQtyTable[2]  = HybridQuantity::Bz;
    hybridQtyTable[3]  = HybridQuantity::Ex;
    hybridQtyTable[4]  = HybridQuantity::Ey;
    hybridQtyTable[5]  = HybridQuantity::Ez;
    hybridQtyTable[6]  = HybridQuantity::Jx;
    hybridQtyTable[7]  = HybridQuantity::Jy;
    hybridQtyTable[8]  = HybridQuantity::Jz;
    hybridQtyTable[9]  = HybridQuantity::rho;
    hybridQtyTable[10] = HybridQuantity::V;
    hybridQtyTable[11] = HybridQuantity::P;

    if (iqty < static_cast<uint32>(HybridQuantity::count))
    {
        quantity = hybridQtyTable[iqty];
    }
    else
    {
        quantity = HybridQuantity::count;
    }

    return quantity;
}


std::string GetHybridQtyName(uint32 iqty)
{
    std::string qtyName{"None"};

    std::array<std::string, static_cast<uint32>(HybridQuantity::count)> hybridQtyNames;

    hybridQtyNames[0]  = "Bx";
    hybridQtyNames[1]  = "By";
    hybridQtyNames[2]  = "Bz";
    hybridQtyNames[3]  = "Ex";
    hybridQtyNames[4]  = "Ey";
    hybridQtyNames[5]  = "Ez";
    hybridQtyNames[6]  = "Jx";
    hybridQtyNames[7]  = "Jy";
    hybridQtyNames[8]  = "Jz";
    hybridQtyNames[9]  = "rho";
    hybridQtyNames[10] = "V";
    hybridQtyNames[11] = "P";

    if (iqty < static_cast<uint32>(HybridQuantity::count))
    {
        qtyName = hybridQtyNames[iqty];
    }

    return qtyName;
}


std::vector<GridLayoutParams> getIndexingInputsFromFile()
{
    std::ifstream ifs{"../GridLayout/gridIndexing.txt"};
    if (!ifs)
        exit(-1);

    uint32 orderMax = 4;
    uint32 dimMax   = 3;

    uint32 nbrTestCases = orderMax * dimMax * static_cast<uint32>(HybridQuantity::count);

    std::vector<GridLayoutParams> params(nbrTestCases);
    uint32 iqty;

    for (uint32 i = 0; i < nbrTestCases; ++i)
    {
        ifs >> params[i].interpOrder >> params[i].nbDim >> iqty >> params[i].nbrCells[0]
            >> params[i].nbrCells[1] >> params[i].nbrCells[2] >> params[i].dxdydz[0]
            >> params[i].dxdydz[1] >> params[i].dxdydz[2] >> params[i].PSI[0] >> params[i].PEI[0]
            >> params[i].GSI[0] >> params[i].GEI[0] >> params[i].PSI[1] >> params[i].PEI[1]
            >> params[i].GSI[1] >> params[i].GEI[1] >> params[i].PSI[2] >> params[i].PEI[2]
            >> params[i].GSI[2] >> params[i].GEI[2];


        params[i].qty  = GetHybridQty(iqty);
        params[i].iqty = iqty;
    }

    return params;
}



std::vector<GridLayoutParams> getAllocInputsFromFile()
{
    std::ifstream ifs{"../GridLayout/allocSizes.txt"};
    if (!ifs)
        exit(-1);

    uint32 orderMax = 4;
    uint32 dimMax   = 3;

    uint32 nbrTestCases = orderMax * dimMax * static_cast<uint32>(HybridQuantity::count);

    std::vector<GridLayoutParams> params(nbrTestCases);
    uint32 iqty;

    for (uint32 i = 0; i < nbrTestCases; ++i)
    {
        ifs >> params[i].interpOrder >> params[i].nbDim >> iqty >> params[i].nbrCells[0]
            >> params[i].nbrCells[1] >> params[i].nbrCells[2] >> params[i].dxdydz[0]
            >> params[i].dxdydz[1] >> params[i].dxdydz[2] >> params[i].allocSizes[0]
            >> params[i].allocSizes[1] >> params[i].allocSizes[2] >> params[i].allocSizeDerived[0]
            >> params[i].allocSizeDerived[1] >> params[i].allocSizeDerived[2];

        params[i].qty  = GetHybridQty(iqty);
        params[i].iqty = iqty;
    }

    return params;
}



std::vector<GridLayoutParams> getFieldCoordsInputsFromFile()
{
    std::ifstream ifs1{"../GridLayout/fieldCoords_summary.txt"};
    if (!ifs1)
    {
        std::cout << "Could not open file : ../GridLayout/fieldCoords_summary.txt" << std::endl;
        exit(-1);
    }

    uint32 nbrTestCases = 0;
    ifs1 >> nbrTestCases;

    std::vector<GridLayoutParams> params(nbrTestCases);

    for (uint32 i = 0; i < nbrTestCases; ++i)
    {
        uint32 iqty;

        ifs1 >> params[i].interpOrder;
        ifs1 >> params[i].nbDim;
        ifs1 >> iqty;
        for (uint32 idim = 0; idim < params[i].nbDim; ++idim)
        {
            ifs1 >> params[i].nbrCells[idim];
            ifs1 >> params[i].dxdydz[idim];
        }
        ifs1 >> params[i].field_iStart;
        ifs1 >> params[i].field_iEnd;
        ifs1 >> params[i].origin.x;
        ifs1 >> params[i].origin.y;
        ifs1 >> params[i].origin.z;

        params[i].qty     = GetHybridQty(iqty);
        params[i].qtyName = GetHybridQtyName(iqty);
        params[i].iqty    = iqty;

        params[i].fieldXCoords.assign(MAX_SIZE, 0.);
    }

    return params;
}



std::vector<GridLayoutParams> getCenteredCoordsInputsFromFile()
{
    std::ifstream ifs1{"../GridLayout/centeredCoords_summary.txt"};
    if (!ifs1)
    {
        std::cout << "Could not open file : ../GridLayout/centeredCoords_summary.txt" << std::endl;
        exit(-1);
    }

    uint32 nbrTestCases = 0;
    ifs1 >> nbrTestCases;

    std::vector<GridLayoutParams> params(nbrTestCases);

    for (uint32 i = 0; i < nbrTestCases; ++i)
    {
        ifs1 >> params[i].interpOrder;
        ifs1 >> params[i].nbDim;

        for (uint32 idim = 0; idim < params[i].nbDim; ++idim)
        {
            ifs1 >> params[i].nbrCells[idim];
            ifs1 >> params[i].dxdydz[idim];
        }

        ifs1 >> params[i].field_iStart;
        ifs1 >> params[i].field_iEnd;
        ifs1 >> params[i].origin.x;
        ifs1 >> params[i].origin.y;
        ifs1 >> params[i].origin.z;

        params[i].cellCenteredXCoords.assign(MAX_SIZE, 0.);
    }

    for (uint32 i = 0; i < nbrTestCases; ++i)
    {
        uint32 order = params[i].interpOrder;
        uint32 dim   = params[i].nbDim;

        std::string filename{"../GridLayout/centeredCoords_ord" + std::to_string(order) + "_dim"
                             + std::to_string(dim) + ".txt"};

        std::cout << filename << std::endl;

        std::ifstream ifs2{filename};
        if (!ifs2)
        {
            std::cout << "Could not open file : " << filename << std::endl;
            exit(-1);
        }

        uint32 iStart = params[i].field_iStart;
        uint32 iEnd   = params[i].field_iEnd;
        for (uint32 ik = iStart; ik < iEnd + 1; ++ik)
        {
            ifs2 >> params[i].cellCenteredXCoords[ik];
        }
    }

    return params;
}
