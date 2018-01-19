
#include <iostream>

#include "test_utils.h"
#include "utilities/hybridenums.h"




void printTable(std::vector<double> const& table, const std::string& name)
{
    std::cout << "- " << name << " -" << std::endl;
    for (uint32 ik = 0; ik < table.size(); ++ik)
    {
        std::cout << " [" << ik << "] = " << table[ik] << ",";
    }
    std::cout << std::endl;
}



std::string GetHybridQtyName(uint32 iqty)
{
    std::string qtyName{"None"};

    std::array<std::string, static_cast<uint32>(HybridQuantity::count)> hybridQtyNames;

    hybridQtyNames[0] = "Bx";
    hybridQtyNames[1] = "By";
    hybridQtyNames[2] = "Bz";
    hybridQtyNames[3] = "Ex";
    hybridQtyNames[4] = "Ey";
    hybridQtyNames[5] = "Ez";
    hybridQtyNames[6] = "rho";
    hybridQtyNames[7] = "V";
    hybridQtyNames[8] = "P";

    if (iqty < static_cast<uint32>(HybridQuantity::count))
    {
        qtyName = hybridQtyNames[iqty];
    }

    return qtyName;
}
