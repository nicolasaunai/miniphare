#include <iostream>
#include <memory>
#include <array>
#include <string>

#include "types.h"
#include "Time/time.h"
#include "AMR/patch.h"
#include "AMR/mlmd.h"
#include "AMR/patchdata.h"
#include "AMR/hierarchy.h"
#include "grid/gridlayout.h"
#include "Diagnostics/diagnostics.h"
#include "Initializer/initializerfactory.h"



int main(int argc, char *argv[])
{
    std::cout << "Welcome to : " << std::endl;

    std::cout <<R"(
                 __  __ _____ _   _ _____ _____  _    _          _____  ______
                |  \/  |_   _| \ | |_   _|  __ \| |  | |   /\   |  __ \|  ____|
                | \  / | | | |  \| | | | | |__) | |__| |  /  \  | |__) | |__
                | |\/| | | | | . ` | | | |  ___/|  __  | / /\ \ |  _  /|  __|
                | |  | |_| |_| |\  |_| |_| |    | |  | |/ ____ \| | \ \| |____
                |_|  |_|_____|_| \_|_____|_|    |_|  |_/_/    \_\_|  \_\______| v1.0)"
             << std::endl << std::endl;




    std::unique_ptr<InitializerFactory> initFactory = fromCommandLine(argc, argv) ;

    Time timeManager{initFactory->timeStep(), 0., 100};
    DiagnosticsManager diagnosticManager{ExportStrategyType::ASCII};

    MLMD mlmdManager{*initFactory} ;
    mlmdManager.initializeRootLevel() ;

    for (uint32 it=0; it < timeManager.nbrIter(); ++it)
    {
        mlmdManager.evolveFullDomain() ;
      //  diagScheduler.applyDiagnostics( timeManager);
        timeManager.advance();

        std::cout << timeManager.currentTime() << std::endl;
    }
}
