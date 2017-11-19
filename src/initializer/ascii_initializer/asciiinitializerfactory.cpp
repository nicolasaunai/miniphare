
#include <string>


#include "asciiinitializerfactory.h"
#include "ini_extractor_factory.h"
#include "initializer/initmodel/init_model_factory.h"
#include <core/BoundaryConditions/boundary_conditions.h>
#include <core/BoundaryConditions/domainboundarycondition.h>
#include <initializer/fluidparticleinitializer.h>

AsciiInitializerFactory::AsciiInitializerFactory(std::string const& filename)
    : iniData_{filename}
    , layout_{{{iniData_.dx, iniData_.dy, iniData_.dz}},
              {{iniData_.nbrCellx, iniData_.nbrCelly, iniData_.nbrCellz}},
              iniData_.nbdims(),
              iniData_.layoutType,
              iniData_.origin,
              iniData_.interpOrder}
    , iniExtractor_{INIExtractorFactory::createINIExtractor(iniData_.modelName)}
    , initModel_{InitModelFactory::createInitModel(iniData_.modelName, layout_)}
    , splitMethods_{std::vector<std::string>{iniData_.nbrSpecies, iniData_.splittingMethod}}
    , interpOrder_{iniData_.interpOrder}
{
    iniExtractor_->initializeModel(iniData_, initModel_.get());
}




std::unique_ptr<IonsInitializer> AsciiInitializerFactory::createIonsInitializer() const
{
    std::vector<uint32> nbrParticlesPerCell = initModel_->nbrsParticlesPerCell();
    std::vector<double> charges             = iniData_.speciesCharges;
    std::vector<double> masses              = iniData_.speciesMasses;

    std::unique_ptr<IonsInitializer> ionInitPtr{new IonsInitializer{}};
    ionInitPtr->nbrSpecies           = iniData_.nbrSpecies;
    ionInitPtr->masses               = iniData_.speciesMasses;
    ionInitPtr->names                = iniData_.speciesNames;
    ionInitPtr->particleInitializers = initModel_->particleInitializers();

    return ionInitPtr;
}




std::unique_ptr<ElectromagInitializer> AsciiInitializerFactory::createElectromagInitializer() const
{
    std::unique_ptr<VectorFunction> magneticField = initModel_->magneticFunction();
    std::unique_ptr<VectorFunction> electricField = initModel_->electricFunction();

    std::unique_ptr<ElectromagInitializer> eminit{new ElectromagInitializer{
        layout_, std::move(electricField), std::move(magneticField), "_EField", "_BField"}};

    std::cout << "creating INI ElectromagInitializer" << std::endl;


    return eminit;
}




std::unique_ptr<SolverInitializer> AsciiInitializerFactory::createSolverInitializer() const
{
    std::unique_ptr<SolverInitializer> solverInitPtr{new SolverInitializer{}};

    solverInitPtr->pusherType         = iniData_.pusherName;
    solverInitPtr->interpolationOrder = iniData_.interpOrder;

    return solverInitPtr;
}



// TODO make this function work for 2D and 3D cases... it is kinda hard coded for now.
std::unique_ptr<BoundaryCondition> AsciiInitializerFactory::createBoundaryCondition() const
{
    // return hard coded domain periodic boundary condition
    std::vector<DomainBoundaryCondition::BoundaryInfo> boundaries(2);

    // "first" is the edge coordinate
    boundaries[0].first = Edge::Xmin;
    boundaries[1].first = Edge::Xmax;

    // TODO to better than this stupid code...
    if (iniData_.boundaryConditionX == "periodic")
    {
        auto bcx = BoundaryType::Periodic;
        // "second" is the type of boundary, here periodic
        boundaries[0].second = bcx;
        boundaries[1].second = bcx;
    }
    else
        throw std::runtime_error("ERROR unknown X boundary condition");


    std::unique_ptr<BoundaryCondition> bc{new DomainBoundaryCondition{layout_, boundaries}};

    return bc;
}




std::unique_ptr<DiagnosticInitializer> AsciiInitializerFactory::createDiagnosticInitializer() const
{
    std::unique_ptr<DiagnosticInitializer> initializer{new DiagnosticInitializer};

    initializer->exportType = ExportStrategyType::ASCII;
    std::vector<uint32> iters;
    for (uint32 i = 0; i < 100001; i += 5)
    {
        iters.push_back(i);
    }

    EMDiagInitializer emDiag;
    emDiag.typeName = "E";
    // emDiag.computingIterations.insert(emDiag.computingIterations.end(), {1,10,20,25});
    // emDiag.writingIterations.insert(emDiag.writingIterations.end(), {1,10,20,25});
    emDiag.computingIterations = iters;
    emDiag.writingIterations   = iters;
    initializer->emInitializers.push_back(std::move(emDiag));

    EMDiagInitializer BDiag;
    BDiag.typeName = "B";
    // BDiag.computingIterations.insert(BDiag.computingIterations.end(), {1,10,20,25});
    // BDiag.writingIterations.insert(BDiag.writingIterations.end(), {1,10,20,25});
    BDiag.computingIterations = iters;
    BDiag.writingIterations   = iters;
    initializer->emInitializers.push_back(std::move(BDiag));


    FluidDiagInitializer fluidDiag;
    fluidDiag.speciesName         = "proton1";
    fluidDiag.typeName            = "rho_s";
    fluidDiag.computingIterations = iters;
    fluidDiag.writingIterations   = iters;
    initializer->fluidInitializers.push_back(std::move(fluidDiag));


    FluidDiagInitializer fluidDiag2;
    fluidDiag2.speciesName         = "proton1";
    fluidDiag2.typeName            = "flux_s";
    fluidDiag2.computingIterations = iters;
    fluidDiag2.writingIterations   = iters;
    initializer->fluidInitializers.push_back(std::move(fluidDiag2));

    // FluidDiagInitializer fluidDiag_p2;
    // fluidDiag_p2.speciesName         = "proton2";
    // fluidDiag_p2.typeName            = "rho_s";
    // fluidDiag_p2.computingIterations = iters;
    // fluidDiag_p2.writingIterations   = iters;
    // initializer->fluidInitializers.push_back(std::move(fluidDiag_p2));


    // FluidDiagInitializer fluidDiag2_p2;
    // fluidDiag2_p2.speciesName         = "proton2";
    // fluidDiag2_p2.typeName            = "flux_s";
    // fluidDiag2_p2.computingIterations = iters;
    // fluidDiag2_p2.writingIterations   = iters;
    // initializer->fluidInitializers.push_back(std::move(fluidDiag2_p2));


    return initializer;
}




std::unique_ptr<Time> AsciiInitializerFactory::createTimeManager() const
{
    return std::unique_ptr<Time>{new Time{iniData_.dt, 0., iniData_.nbrSteps * iniData_.dt}};
}
