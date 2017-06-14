#include <array>
#include <iostream>
#include <memory>
#include <string>

#include "Time/time.h"
#include "types.h"

#include "AMR/Hierarchy/hierarchy.h"
#include "AMR/MLMD/mlmd.h"
#include "AMR/patch.h"
#include "AMR/patchdata.h"

#include "Diagnostics/diagnosticmanager.h"
#include "Initializer/simulationinitializerfactory.h"
#include "grid/gridlayout.h"



int main(int argc, char *argv[])
{
    std::cout << "Welcome to : " << std::endl;

    std::cout << R"(
                 __  __ _____ _   _ _____ _____  _    _          _____  ______
                |  \/  |_   _| \ | |_   _|  __ \| |  | |   /\   |  __ \|  ____|
                | \  / | | | |  \| | | | | |__) | |__| |  /  \  | |__) | |__
                | |\/| | | | | . ` | | | |  ___/|  __  | / /\ \ |  _  /|  __|
                | |  | |_| |_| |\  |_| |_| |    | |  | |/ ____ \| | \ \| |____
                |_|  |_|_____|_| \_|_____|_|    |_|  |_/_/    \_\_|  \_\______| v1.0)"
              << std::endl
              << std::endl;




    std::unique_ptr<SimulationInitializerFactory> initFactory = fromCommandLine(argc, argv);


    std::unique_ptr<Time> timeManager{initFactory->createTimeManager()};
    DiagnosticsManager diagnosticManager{initFactory->createDiagnosticInitializer()};


    Hierarchy patchHierarchy{std::make_shared<Patch>(
        initFactory->getBox(), initFactory->gridLayout(), PatchData{*initFactory})};

    MLMD mlmdManager{*initFactory};
    mlmdManager.initializeRootLevel(patchHierarchy);


    for (uint32 it = 0; it < timeManager->nbrIter(); ++it)
    {
        //std::cout << it << std::endl;
        std::cout << timeManager->currentTime() << std::endl;

        mlmdManager.evolveFullDomain(patchHierarchy);
        diagnosticManager.compute(*timeManager, patchHierarchy);
        diagnosticManager.save(*timeManager);
        timeManager->advance();
    }
}
