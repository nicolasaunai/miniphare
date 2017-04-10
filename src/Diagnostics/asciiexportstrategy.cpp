
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <iomanip>
#include <stdio.h>

#include "asciiexportstrategy.h"



void AsciiExportStrategy::saveEMDiagnostic(EMDiagnostic const& diag, Time const& timeManager)
{

}


void AsciiExportStrategy::saveFluidDiagnostic(FluidDiagnostic const& diag, Time const& timeManager)
{
    std::cout << "I'm writting fluid diagnostics at t = " << timeManager.currentTime()
              << std::endl;
}
