
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <iomanip>
#include <stdio.h>

#include "asciiexportstrategy.h"



float centering2float(QtyCentering centering)
{
    if(QtyCentering::dual == centering)
    {
        return 0.5;
    }
    else
        return 0;
}


void fillFile(FieldPack const& pack, FILE* file)
{

    for (uint32 iField = 0; iField < pack.data.size();  ++iField)
    {
        std::string key = pack.keys[iField];

        fprintf(file, "#nbrDimensions\n");
        auto nbdims = pack.nbrDimensions.find(key); // [] cannot be used for const unordered_map
        fprintf(file, "%d", nbdims->second);
        fprintf(file, "\n\n\n");

        fprintf(file, "# Origin\n");
        auto itOri = pack.origin.find(key);
        fprintf(file, "%f %f %f", itOri->second.x_, itOri->second.y_, itOri->second.z_);
        fprintf(file, "\n\n\n");

        fprintf(file, "# nbrNoxdes x y z\n");
        auto itNbrNodes = pack.nbrNodes.find(key);
        for (uint32 n : itNbrNodes->second)
        {
            fprintf(file, "%d ", n);
        }
        fprintf(file, "\n\n\n");

        fprintf(file, "# centering x y z\n");
        auto itCentering = pack.centerings.find(key);
        for (auto centering : itCentering->second)
        {
            fprintf(file, "%f ", centering2float(centering));
        }
        fprintf(file, "\n\n\n");


        fprintf(file, "# data\n");
        auto itData = pack.data.find(key);
        fprintf(file, "#%s %d \n", itData->first.c_str(), static_cast<int>(itData->second.size()));
        for (uint i=0; i < itData->second.size(); ++i)
        {
            fprintf(file, "%f\n", itData->second[i]);
        }
        fprintf(file, "# ------------------------------------------\n\n\n");
    }
}




/* ----------------------------------------------------------------------------

                         ELECTROMAGNETIC DIAGNOSTICS

   ---------------------------------------------------------------------------- */

std::string getEMFilename(uint32 patchID,
                          EMDiagnostic const& diag,
                          Time const& timeManager)
{
    std::stringstream ss;
    ss << diag.stratName()
       << "_" << std::setfill('0') << std::setw(6)
       << "_" << timeManager.currentTime()
       << patchID << ".txt";
    return ss.str();
}




void AsciiExportStrategy::saveEMDiagnostic(EMDiagnostic const& diag, Time const& timeManager)
{
    std::cout << "I'm writting EM diagnostics at t = "
              << timeManager.currentTime()
              << " Diag type : "  << diag.stratName()
              << std::endl;


    uint32 pacthID=0;
    // there is one FieldPack per Patch
    // we save one file per Patch.
    for (FieldPack const& pack : diag.getPacks())
    {
        std::string filename = getEMFilename(pacthID, diag, timeManager);
        FILE *file = fopen(filename.c_str(), "w");
        fillFile(pack, file);
        fclose(file);
        pacthID++;
    }
}



/* ----------------------------------------------------------------------------

                             FLUID DIAGNOSTICS

   ---------------------------------------------------------------------------- */





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




void AsciiExportStrategy::saveFluidDiagnostic(FluidDiagnostic const& diag, Time const& timeManager)
{
    std::cout << "I'm writting fluid diagnostics for species "
              << diag.speciesName() << "at t = "
              << timeManager.currentTime()
              << " Diag type : "  << diag.stratName()
              << std::endl;


    uint32 pacthID=0;
    // there is one FieldPack per Patch
    // we save one file per Patch.
    for (FieldPack const& pack : diag.getPacks())
    {
        std::string filename = getFluidFilename(pacthID, diag, timeManager);
        FILE *file = fopen(filename.c_str(), "w");
        fillFile(pack, file);
        fclose(file);
        pacthID++;
    }
}
