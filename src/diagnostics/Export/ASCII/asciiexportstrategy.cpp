
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#include "asciiexportstrategy.h"

#include "utilities/particleutilities.h"


float centering2float(QtyCentering centering)
{
    if (QtyCentering::dual == centering)
    {
        return 0.5;
    }
    else
        return 0;
}


void fillFile(FieldPack const& pack, FILE* file)
{
    for (uint32 iField = 0; iField < pack.data.size(); ++iField)
    {
        std::string key = pack.keys[iField];

        fprintf(file, "# nbrDimensions\n");
        auto nbdims = pack.nbrDimensions.find(key); // [] cannot be used for const unordered_map
        fprintf(file, "%d", nbdims->second);
        fprintf(file, "\n");

        fprintf(file, "# Origin\n");
        auto itOri = pack.origin.find(key);
        fprintf(file, "%f %f %f", itOri->second.x, itOri->second.y, itOri->second.z);
        fprintf(file, "\n");

        fprintf(file, "# grid spacing\n");
        auto spacing = pack.gridSpacing.find(key);
        fprintf(file, "%f %f %f", spacing->second[0], spacing->second[1], spacing->second[2]);
        fprintf(file, "\n");

        fprintf(file, "# nbrNoxdes x y z\n");
        auto itNbrNodes = pack.nbrNodes.find(key);
        for (uint32 n : itNbrNodes->second)
        {
            fprintf(file, "%d ", n);
        }
        fprintf(file, "\n");

        fprintf(file, "# centering x y z\n");
        auto itCentering = pack.centerings.find(key);
        for (auto centering : itCentering->second)
        {
            fprintf(file, "%f ", centering2float(centering));
        }
        fprintf(file, "\n");


        fprintf(file, "# data\n");
        auto itData = pack.data.find(key);
        fprintf(file, "#%s %d \n", itData->first.c_str(), static_cast<int>(itData->second.size()));
        for (uint32 i = 0; i < itData->second.size(); ++i)
        {
            fprintf(file, "%f\n", itData->second[i]);
        }
        fprintf(file, "# ------------------------------------------\n\n\n");
    }
}


void fillFile(ParticlePack const& pack, FILE* file)
{
    // writing local diagnostic informations
    fprintf(file, "# Origin\n");
    auto const& origin = pack.origin;
    fprintf(file, "%f %f %f", origin.x, origin.y, origin.z);
    fprintf(file, "\n");

    fprintf(file, "# grid spacing\n");
    auto const& spacing = pack.gridSpacing;
    fprintf(file, "%f %f %f", spacing[0], spacing[1], spacing[2]);
    fprintf(file, "\n");

    fprintf(file, "# number of particles\n");
    fprintf(file, "%lu", pack.nbParticles);
    fprintf(file, "\n");

    for (Particle const& part : pack.data)
    {
        Point coord = getParticlePosition(part, origin, pack.nbrGhosts, spacing);

        // write weight and charge
        fprintf(file, "%g %f \n", part.weight, part.charge);
        // write particle coordinates
        fprintf(file, "%e %e %e \n", coord.x, coord.y, coord.z);
        // write velocities
        fprintf(file, "%e %e %e \n", part.v[0], part.v[1], part.v[2]);
        fflush(stdout);
    }
}


/* ----------------------------------------------------------------------------

                         ELECTROMAGNETIC DIAGNOSTICS

   ---------------------------------------------------------------------------- */

std::string getEMFilename(uint32 patchID, EMDiagnostic const& diag, Time const& timeManager)
{
    std::stringstream ss;
    ss << diag.path() << "/" << diag.name() << "_" << std::setfill('0') << std::setw(6) << patchID
       << "_" << std::setprecision(6) << std::scientific << timeManager.currentTime() << ".txt";

    return ss.str();
}




void AsciiExportStrategy::saveEMDiagnostic(EMDiagnostic const& diag, Time const& timeManager)
{
    std::cout << "I'm writting EM diagnostics at t = " << timeManager.currentTime()
              << " Diag type : " << diag.stratName() << std::endl;


    uint32 pacthID = 0;
    // there is one FieldPack per Patch
    // we save one file per Patch.
    for (FieldPack const& pack : diag.getPacks())
    {
        std::string filename = getEMFilename(pacthID, diag, timeManager);
        FILE* file           = fopen(filename.c_str(), "w");
        fillFile(pack, file);
        fclose(file);
        pacthID++;
    }
}



/* ----------------------------------------------------------------------------

                             FLUID DIAGNOSTICS

   ---------------------------------------------------------------------------- */


std::string getFluidFilename(uint32 patchID, FluidDiagnostic const& diag, Time const& timeManager)
{
    std::stringstream ss;

    ss << diag.path() << "/" << diag.name() << '_' << std::setfill('0') << std::setw(6) << patchID
       << "_" << std::setprecision(6) << std::scientific << timeManager.currentTime() << ".txt";

    return ss.str();
}




void AsciiExportStrategy::saveFluidDiagnostic(FluidDiagnostic const& diag, Time const& timeManager)
{
    std::cout << "I'm writting fluid diagnostics for species " << diag.speciesName()
              << "at t = " << timeManager.currentTime() << " Diag type : " << diag.stratName()
              << std::endl;


    uint32 pacthID = 0;
    // there is one FieldPack per Patch
    // we save one file per Patch.
    for (FieldPack const& pack : diag.getPacks())
    {
        std::string filename = getFluidFilename(pacthID, diag, timeManager);
        FILE* file           = fopen(filename.c_str(), "w");
        fillFile(pack, file);
        fclose(file);
        pacthID++;
    }
}



/* ----------------------------------------------------------------------------

                             PARTICLE DIAGNOSTICS

   ---------------------------------------------------------------------------- */


std::string getParticleFilename(uint32 patchID, ParticleDiagnostic const& diag,
                                Time const& timeManager)
{
    std::stringstream ss;

    ss << diag.path() << "/" << diag.name() << "_" << diag.speciesName() << '_' << std::setfill('0')
       << std::setw(2) << patchID << "_" << std::setprecision(6) << std::scientific
       << timeManager.currentTime() << ".txt";

    return ss.str();
}



void AsciiExportStrategy::saveParticleDiagnostic(ParticleDiagnostic const& diag,
                                                 Time const& timeManager)
{
    std::cout << "I'm writting fluid diagnostics for species " << diag.speciesName()
              << "at t = " << timeManager.currentTime() << " Diag type : " << diag.stratName()
              << std::endl;


    uint32 pacthID = 0;
    // there is one FieldPack per Patch
    // we save one file per Patch.
    for (ParticlePack const& pack : diag.getPacks())
    {
        std::string filename = getParticleFilename(pacthID, diag, timeManager);
        FILE* file           = fopen(filename.c_str(), "w");
        fillFile(pack, file);
        fclose(file);
        pacthID++;
    }
}
