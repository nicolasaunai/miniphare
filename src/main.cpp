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

#include "utilities/print/outputs.h"


int main(int argc, char* argv[])
{
    Logger::setVerbosity(Logger::NormalVerbosity);
    Logger::Info << "Welcome to : \n";

    Logger::Info << R"(
                 __  __ _____ _   _ _____ _____  _    _          _____  ______
                |  \/  |_   _| \ | |_   _|  __ \| |  | |   /\   |  __ \|  ____|
                | \  / | | | |  \| | | | | |__) | |__| |  /  \  | |__) | |__
                | |\/| | | | | . ` | | | |  ___/|  __  | / /\ \ |  _  /|  __|
                | |  | |_| |_| |\  |_| |_| |    | |  | |/ ____ \| | \ \| |____
                |_|  |_|_____|_| \_|_____|_|    |_|  |_/_/    \_\_|  \_\______| v1.0)"
                 << "\n\n";




    Logger::Info << Logger::hline;
    Logger::Info << "INITIALIZATION\n";
    Logger::Info << Logger::hline;

    std::unique_ptr<SimulationInitializerFactory> initFactory = fromCommandLine(argc, argv);


    std::unique_ptr<Time> timeManager{initFactory->createTimeManager()};
    DiagnosticsManager diagnosticManager{initFactory->createDiagnosticInitializer()};


    Hierarchy patchHierarchy{std::make_shared<Patch>(initFactory->getBox(), initFactory->timeStep(),
                                                     initFactory->gridLayout(),
                                                     PatchData{*initFactory})};

    MLMD mlmdManager{*initFactory};
    mlmdManager.initializeRootLevel(patchHierarchy);

    Logger::Info << Logger::hline;
    Logger::Info << "STARTING MAIN SIMULATION LOOP\n";
    Logger::Info << Logger::hline;
    Logger::Info.flush();


    for (uint32 it = 0; it < timeManager->nbrIter(); ++it)
    {
        Logger::Info << "time/Iteration : " << timeManager->currentTime() << " / "
                     << timeManager->currentIteration() << "\n";
        Logger::Info.flush();

        mlmdManager.evolveFullDomain(patchHierarchy, it);
        diagnosticManager.compute(*timeManager, patchHierarchy);
        diagnosticManager.save(*timeManager);
        timeManager->advance();

        Logger::Info << Logger::sharpLine;
        Logger::Info.flush();
    }
}
