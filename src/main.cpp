#include <iostream>
#include <memory>
#include <array>
#include <string>

#include "types.h"
#include "Time/time.h"

#include "AMR/patch.h"
#include "AMR/MLMD/mlmd.h"
#include "AMR/patchdata.h"
#include "AMR/Hierarchy/hierarchy.h"

#include "grid/gridlayout.h"
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


    Hierarchy patchHierarchy{ std::make_shared<Patch>(initFactory->getBox(),
                                                       initFactory->gridLayout(),
                                                       PatchData{*initFactory}  ) };

    Time timeManager{initFactory->timeStep(), 0., 100};
    MLMD mlmdManager{*initFactory} ;

    mlmdManager.initializeRootLevel(patchHierarchy) ;

    for (uint32 it=0; it < timeManager.nbrIter(); ++it)
    {

        mlmdManager.evolveFullDomain(patchHierarchy) ;
        std::cout << it << std::endl;
        std::cout << timeManager.currentTime() << std::endl;
        timeManager.advance();
    }

}



