
#include <string>
#include <iostream>
#include "asciiexportstrategy.h"
#include <stdio.h>



void AsciiExportStrategy::save(Diagnostic const& diag, Time const& timeManager)
{
    std::string const& name = diag.name();
    double time = timeManager.currentTime();

    for (DiagPack const& pack : diag.data())
    {
        std::vector<std::string> dependsKeys;
        std::vector<std::vector<float>> depends;

        for (auto& dep : pack.depends)
        {
            dependsKeys.push_back(std::move(dep.first));
            depends.push_back(std::move(dep.second));
        }

        std::vector<std::string> dataKeys;
        std::vector<std::vector<float>> datasets;

        for (auto& data : pack.data)
        {
            dataKeys.push_back(std::move(data.first));
            datasets.push_back(std::move(data.second));
        }

        std::vector<std::string> nbrNodesKeys;
        std::vector<std::array<uint32, 3>> nbrNodesValues;

        for (auto& nbrNodes : pack.nbrNodes)
        {
            nbrNodesKeys.push_back(std::move(nbrNodes.first));
            nbrNodesValues.push_back(std::move(nbrNodes.second));
        }

        for (uint32 iDataset=0; iDataset < datasets.size(); ++iDataset)
        {
            std::string  filename = dataKeys[iDataset] + ".txt";
            std::cout << filename << std::endl;

            FILE *fp = fopen(filename.c_str(), "w");

            uint32 nbrValues = static_cast<uint32>(datasets[iDataset].size());

            // for all values in the dataset
            for (uint32 iVal =0; iVal < nbrValues; ++iVal)
            {
                // first write on the same line all dependences variables
                for (uint32 iDep=0; iDep < depends.size(); ++iDep)
                {
                    fprintf(fp, "%f ", depends[iDep][iVal]);
                }
                // then continue the line by the dataset value and end the line
                fprintf(fp, "%f\n", datasets[iDataset][iVal]);
            }

            fclose(fp);
        }

    }
    // patchID_DiagID_Time.txt

   // std::vector<DiagData> const& data = diag.data();


    // open file

    // now write data
   // fprintf(fp, "", di);

    // close file
}
