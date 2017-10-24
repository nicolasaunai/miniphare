#include <array>
#include <iostream>
#include <memory>
#include <string>

#include "types.h"
#include "utilities/Time/pharetime.h"

#include "amr/Hierarchy/hierarchy.h"
#include "amr/MLMD/mlmd.h"
#include "amr/Patch/patch.h"
#include "amr/Patch/patchdata.h"

#include "diagnostics/diagnosticmanager.h"
#include "initializer/simulationinitializerfactory.h"




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


    Hierarchy patchHierarchy{std::make_shared<Patch>(initFactory->getBox(), initFactory->timeStep(),
                                                     initFactory->gridLayout(),
                                                     PatchData{*initFactory})};

    MLMD mlmdManager{*initFactory};
    mlmdManager.initializeRootLevel(patchHierarchy);


    for (uint32 it = 0; it < timeManager->nbrIter(); ++it)
    {
        std::cout << "itime = " << it << std::endl;

        std::cout << timeManager->currentTime() << std::endl;

        mlmdManager.evolveFullDomain(patchHierarchy, it);
        diagnosticManager.compute(*timeManager, patchHierarchy);
        diagnosticManager.save(*timeManager);
        timeManager->advance();
    }
}
