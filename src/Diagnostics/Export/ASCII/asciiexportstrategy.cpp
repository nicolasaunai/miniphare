
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



std::string getFluidFilename(uint32 patchID,
                             FluidDiagnostic const& diag,
                             Time const& timeManager)
{
    std::stringstream ss;
    ss << diag.stratName() << "_" << diag.speciesName()
       << "_" << std::setfill('0') << std::setw(6)
       << "_" << timeManager.currentTime()
       << patchID << ".txt";
    return ss.str();
}



float centering2float(QtyCentering centering)
{
    if(QtyCentering::dual == centering)
    {
        return 0.5;
    }
    else
        return 0;
}


void AsciiExportStrategy::saveFluidDiagnostic(FluidDiagnostic const& diag, Time const& timeManager)
{
    std::cout << "I'm writting fluid diagnostics for species "
              << diag.speciesName() << "at t = "
              << timeManager.currentTime()
              << " Diag type : "  << diag.stratName()
              << std::endl;


    uint32 pacthID=0;
    for (FieldPack const& pack : diag.getPacks())
    {
        std::string filename = getFluidFilename(pacthID, diag, timeManager);
        FILE *file = fopen(filename.c_str(), "w");

        fprintf(file, "#nbrDimensions\n");
        fprintf(file, "%d", pack.nbrDimensions);
        fprintf(file, "\n\n\n");

        fprintf(file, "# Origin\n");
        fprintf(file, "%f %f %f", pack.origin.x_, pack.origin.y_, pack.origin.z_);
        fprintf(file, "\n\n\n");



        fprintf(file, "# nbrNodes x y z\n");
        for (uint32 n : pack.nbrNodes)
        {
            fprintf(file, "%d ", n);
        }
        fprintf(file, "\n\n\n");


        fprintf(file, "# centering x y z\n");
        for (auto centering : pack.centerings)
        {
            fprintf(file, "%f ", centering2float(centering));
        }
        fprintf(file, "\n\n\n");





        fprintf(file, "# data\n");
        for (uint i=0; i < pack.data.size(); ++i)
        {
            fprintf(file, "%f\n", pack.data[i]);
        }
        fclose(file);
        pacthID++;
    }
}
