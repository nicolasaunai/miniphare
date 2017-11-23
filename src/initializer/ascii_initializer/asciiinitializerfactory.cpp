

#include <string>

#include "asciiinitializerfactory.h"
#include "ini_extractor_factory.h"
#include "initializer/initmodel/init_model_factory.h"
#include <core/BoundaryConditions/boundary_conditions.h>
#include <core/BoundaryConditions/domainboundarycondition.h>
#include <initializer/fluidparticleinitializer.h>
#include <utilities/utilities.h>

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

    std::unordered_map<std::string, DiagInfos> const& diagInfos = iniData_.diagInfos;
    std::unordered_map<std::string, std::vector<std::size_t>> diagMap;



    for (auto const& infoPair : diagInfos)
    {
        auto diagName   = infoPair.first;
        DiagInfos infos = infoPair.second;

        if (infos.diagCategory == "FluidDiagnostics")
        {
            FluidDiagInitializer fluidDiag;
            fluidDiag.typeName    = infos.diagType;
            fluidDiag.diagName    = infos.diagName;
            fluidDiag.speciesName = infos.speciesName;
            fluidDiag.path        = infos.path;
            fluidDiag.computingIterations
                = arange<uint32>(infos.iStart, infos.iEnd + infos.computeEvery, infos.computeEvery);
            fluidDiag.writingIterations
                = arange<uint32>(infos.iStart, infos.iEnd + infos.writeEvery, infos.writeEvery);
            initializer->fluidInitializers.push_back(std::move(fluidDiag));
        }

        else if (infos.diagCategory == "ElectromagDiagnostics")
        {
            EMDiagInitializer emDiag;
            emDiag.diagName = infos.diagName;
            emDiag.typeName = infos.diagType;
            emDiag.path     = infos.path;
            emDiag.computingIterations
                = arange<uint32>(infos.iStart, infos.iEnd + infos.computeEvery, infos.computeEvery);
            emDiag.writingIterations
                = arange<uint32>(infos.iStart, infos.iEnd + infos.writeEvery, infos.writeEvery);
            initializer->emInitializers.push_back(std::move(emDiag));
        }
    }


    // here add ParticleDiagInitializer

    return initializer;
}




std::unique_ptr<Time> AsciiInitializerFactory::createTimeManager() const
{
    return std::unique_ptr<Time>{new Time{iniData_.dt, 0., iniData_.nbrSteps * iniData_.dt}};
}
