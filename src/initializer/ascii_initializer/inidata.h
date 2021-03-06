#ifndef INIDATA_H
#define INIDATA_H


#include "INIReader.h" // https://github.com/jtilly/inih

#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>


#include <utilities/types.h>


struct MLMDIniData
{
    double minRatio;
    double maxRatio;
    std::string refineAtIteration;
    std::string levelToRefine;
    std::string patchToRefine;
};


struct DiagInfos
{
    int iStart;
    int iEnd;
    int computeEvery;
    int writeEvery;
    std::string diagCategory;
    std::string diagType;
    std::string diagName;
    std::string speciesName;
    std::string path;

    // specific for particle diagnostics
    std::vector<double> selectorParams;
};


/**
 * @brief encapsulate informations read from an INI file.
 */
class INIData
{
public:
    INIData(std::string filename)
        : reader{filename}
    {
        // read all data that is not specific to any particular intial model
        // but is useful for all hybrid simulations
        if (reader.ParseError() >= 0)
        {
            dx = reader.GetReal("simulation", "dx", 0);
            dy = reader.GetReal("simulation", "dy", 0);
            dz = reader.GetReal("simulation", "dz", 0);

            nbrCellx = static_cast<uint32>(reader.GetInteger("simulation", "numOfCellx", 0));
            nbrCelly = static_cast<uint32>(reader.GetInteger("simulation", "numOfCelly", 0));
            nbrCellz = static_cast<uint32>(reader.GetInteger("simulation", "numOfCellz", 0));

            origin.x = reader.GetInteger("simulation", "originx", 0);
            origin.y = reader.GetInteger("simulation", "originy", 0);
            origin.z = reader.GetInteger("simulation", "originz", 0);

            interpOrder = static_cast<uint32>(reader.GetInteger("simulation", "interporder", 1));


            modelName  = reader.Get("model", "model_name", "ERROR_NO_MODEL");
            nbrSpecies = static_cast<uint32>(reader.GetInteger("model", "nbrionpopulations", 1));

            // browse species and get their charge, mass and initial model name
            for (auto speciesIndex = 0u; speciesIndex < nbrSpecies; ++speciesIndex)
            {
                auto indexStr     = std::to_string(speciesIndex);
                auto mass         = reader.GetReal("model", "mass" + indexStr, 1);
                auto charge       = reader.GetReal("model", "charge" + indexStr, 1);
                auto nbrParticles = reader.GetInteger("model", "nbrParticlesPerCell" + indexStr, 1);
                auto name = reader.Get("model", "speciesName" + indexStr, "NO_NAME" + indexStr);
                speciesMasses.push_back(mass);
                speciesCharges.push_back(charge);
                nbrParticlesPerCell.push_back(static_cast<uint32>(nbrParticles));
                speciesNames.push_back(std::move(name));
            }

            boundaryConditionX = reader.Get("simulation", "boundaryconditionx", "periodic");
            boundaryConditionY = reader.Get("simulation", "boundaryconditiony", "periodic");
            boundaryConditionZ = reader.Get("simulation", "boundaryconditionz", "periodic");

            layoutType = reader.Get("simulation", "layoutype", "yee");

            dt = reader.GetReal("simulation", "timestep", 0);

            nbrSteps = static_cast<uint32>(reader.GetInteger("simulation", "numOfSteps", 0));

            pusherName = reader.Get("simulation", "particlepusher", "boris");

            splittingMethod = reader.Get("simulation", "splittingMethod", "splitOrderN_RF2");

            ndims = 3;
            if (nbrCellz == 0)
            {
                ndims--;
                if (nbrCelly == 0)
                    ndims--;
            }

            exportStrategy = reader.Get("simulation", "diagExportType", "ascii");

            // MLMD informations
            MLMDIniData infos;

            infos.minRatio          = reader.GetReal("mlmd", "minratio", 0.4);
            infos.maxRatio          = reader.GetReal("mlmd", "maxratio", 0.6);
            infos.refineAtIteration = reader.Get("mlmd", "refineatiteration", "");
            infos.levelToRefine     = reader.Get("mlmd", "leveltorefine", "");
            infos.patchToRefine     = reader.Get("mlmd", "patchtorefine", "");

            mlmdIniData = std::move(infos);

            auto sections = reader.Sections();
            for (auto section : sections)
            {
                // search for Diagnostics blocks
                // section names are (unkonwn) diagnostic names so we search for
                // any section that is not a standard section name (e.g. 'simulation')
                if (section != "simulation" && section != "model" && section != "mlmd")
                {
                    DiagInfos infos;
                    infos.diagType = reader.Get(section, "diagType", "ERROR_WRONG_DIAGTYPE");
                    // the diagName is the section name
                    infos.diagName     = section;
                    infos.diagCategory = reader.Get(section, "diagCategory", "ERROR_NO_DIAGNAME");
                    infos.writeEvery
                        = static_cast<int>(reader.GetInteger(section, "writeEvery", -1));
                    infos.computeEvery
                        = static_cast<int>(reader.GetInteger(section, "computeEvery", -1));
                    infos.iStart      = static_cast<int>(reader.GetInteger(section, "iStart", -1));
                    infos.iEnd        = static_cast<int>(reader.GetInteger(section, "iEnd", -1));
                    infos.speciesName = reader.Get(section, "speciesName", "NO_SPECIES_NAME");

                    // default path for diagnostics is their name
                    infos.path = reader.Get(section, "path", infos.diagName);

                    std::string category{infos.diagCategory};
                    std::transform(infos.diagCategory.begin(), infos.diagCategory.end(),
                                   category.begin(), ::tolower);

                    if (category == "particlediagnostics")
                    {
                        std::string params_str;

                        std::string diagtype{infos.diagType};
                        std::transform(infos.diagType.begin(), infos.diagType.end(),
                                       diagtype.begin(), ::tolower);

                        if (diagtype == "spacebox")
                        {
                            params_str = reader.Get(section, "spaceparams", "0., 0.");

                            std::istringstream sstream(params_str);
                            std::string token;

                            while (std::getline(sstream, token, ','))
                                infos.selectorParams.push_back(std::stod(token));
                        }
                    }

                    diagInfos[section] = std::move(infos);
                }
            }
        }
    }

    uint32 nbdims() { return ndims; }



    bool error() const { return reader.ParseError() < 0; }

    double dx;
    double dy;
    double dz;
    uint32 nbrCellx;
    uint32 nbrCelly;
    uint32 nbrCellz;
    uint32 ndims;
    Point origin;
    uint32 interpOrder;
    std::string pusherName;
    std::string boundaryConditionX;
    std::string boundaryConditionY;
    std::string boundaryConditionZ;
    std::string layoutType;
    uint32 nbrSpecies;
    std::string splittingMethod;
    std::vector<std::string> speciesNames;
    std::vector<double> speciesMasses;
    std::vector<double> speciesCharges;
    std::vector<uint32> nbrParticlesPerCell;
    std::string modelName;
    double dt;
    uint32 nbrSteps;

    std::string exportStrategy;
    std::unordered_map<std::string, DiagInfos> diagInfos;
    MLMDIniData mlmdIniData;


    INIReader reader;
};



#endif // INIDATA_H
