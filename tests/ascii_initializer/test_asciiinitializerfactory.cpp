
#include <array>
#include <memory>
#include <vector>

#include "initializer/ascii_initializer/asciiinitializerfactory.h"
#include "test_asciiinitializerfactory.h"
#include <data/Plasmas/ionsinitializer.h>
#include <data/Plasmas/particleinitializer.h>
#include <data/Plasmas/particles.h>
#include <utilities/Time/pharetime.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"


test_asciiinitializerfactory::test_asciiinitializerfactory()
{
}


TEST(AsciiInitializerTest, isASimulationFactory)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
}


TEST(AsciiInitializerTest, canReadFile)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    ASSERT_EQ(factory->inputError(), false);
}




TEST(AsciiInitializerTest, returnsGridLayoutOK)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    GridLayout const& layout = factory->gridLayout();
    ASSERT_EQ(0.1, layout.dx());
    ASSERT_EQ(0.0, layout.dy());
    ASSERT_EQ(0.0, layout.dz());
    ASSERT_EQ(0.0, layout.origin().x);
    ASSERT_EQ(0.0, layout.origin().y);
    ASSERT_EQ(0.0, layout.origin().z);
    ASSERT_EQ("yee", layout.layoutName());
}

TEST(AsciiInitializerTest, electromagInitializerNotNull)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<ElectromagInitializer> eminit = factory->createElectromagInitializer();
    ASSERT_NE(eminit, nullptr);
}


TEST(AsciiInitializerTest, nbrSpeciesInIonInitializerOK)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<IonsInitializer> ionInit = factory->createIonsInitializer();

    ASSERT_EQ(2, ionInit->masses.size());
    ASSERT_EQ(2, ionInit->names.size());
    ASSERT_EQ(2, ionInit->particleInitializers.size());
    ASSERT_EQ(2, ionInit->nbrSpecies);
}


TEST(AsciiInitializerTest, massesAreOK)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<IonsInitializer> ionInit = factory->createIonsInitializer();

    ASSERT_EQ(1, ionInit->masses[0]);
    ASSERT_EQ(1, ionInit->masses[1]);
}



TEST(AsciiInitializerTest, splittingMethod)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    ASSERT_EQ("splitOrderN_RF2", factory->splittingStrategies()[0]);
}




TEST(AsciiInitializerTest, numberOfLoadedParticlesOK)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<IonsInitializer> ionInit = factory->createIonsInitializer();

    auto& partInits          = ionInit->particleInitializers;
    GridLayout const& layout = factory->gridLayout();

    auto nbCells    = layout.nbrCellxyz();
    auto nbCellsTot = nbCells[0];

    if (layout.nbDimensions() == 2)
    {
        nbCellsTot *= nbCells[1];
    }
    else if (layout.nbDimensions() == 3)
    {
        nbCellsTot *= nbCells[2];
    }

    uint32 nbrPartPerCell = 100;

    for (std::size_t i = 0; i < partInits.size(); ++i)
    {
        std::vector<Particle> particles;
        partInits[i]->loadParticles(particles);
        ASSERT_EQ(nbrPartPerCell * nbCellsTot, particles.size());
    }
}



TEST(AsciiInitializerTest, speciesNamesOK)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<IonsInitializer> ionInit = factory->createIonsInitializer();
    auto names                               = ionInit->names;

    ASSERT_EQ("proton1", names[0]);
    ASSERT_EQ("proton2", names[1]);
}




TEST(AsciiInitializerTest, SolverInitializerHasTheRightPusher)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<SolverInitializer> solverInit = factory->createSolverInitializer();

    ASSERT_EQ("modifiedBoris", solverInit->pusherType);
}



TEST(AsciiInitializerTest, SolverInitializerHasInterpolationOrdersOK)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<SolverInitializer> solverInit = factory->createSolverInitializer();
    auto interpOrder                              = solverInit->interpolationOrder;
    ASSERT_EQ(1, interpOrder);
}




TEST(AsciiInitializerTest, timeStepIsOK)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<SolverInitializer> solverInit = factory->createSolverInitializer();
    std::unique_ptr<Time> timeManager             = factory->createTimeManager();

    ASSERT_EQ(0.001, timeManager->timeStep());
}



TEST(AsciiInitializerTest, numberOfTimeStepsIsOK)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<SolverInitializer> solverInit = factory->createSolverInitializer();
    std::unique_ptr<Time> timeManager             = factory->createTimeManager();

    ASSERT_EQ(1000, timeManager->nbrIter());
}




TEST(AsciiInitializerTest, EMInitializersNumberIsOK)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<DiagnosticInitializer> diagInit = factory->createDiagnosticInitializer();
    ASSERT_EQ(2, diagInit->emInitializers.size());
}

TEST(AsciiInitializerTest, FluidInitializersNumberIsOK)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<DiagnosticInitializer> diagInit = factory->createDiagnosticInitializer();
    ASSERT_EQ(2, diagInit->fluidInitializers.size());
}


TEST(AsciiInitializerTest, FluidInitializersTypesAreOK)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<DiagnosticInitializer> diagInit = factory->createDiagnosticInitializer();
    for (auto i : std::array<std::size_t, 2>{{0, 1}})
    {
        if (diagInit->fluidInitializers[i].diagName == "FluidDiagnostics1")
        {
            ASSERT_EQ("rho_s", diagInit->fluidInitializers[i].typeName);
        }
        else if (diagInit->fluidInitializers[i].diagName == "FluidDiagnostics1")
        {
            ASSERT_EQ("flux_s", diagInit->fluidInitializers[i].typeName);
        }
    }
}



TEST(AsciiInitializerTest, FluidInitializersSpeciesNamesAreOK)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<DiagnosticInitializer> diagInit = factory->createDiagnosticInitializer();
    ASSERT_EQ("proton1", diagInit->fluidInitializers[0].speciesName);
    ASSERT_EQ("proton1", diagInit->fluidInitializers[1].speciesName);
}


TEST(AsciiInitializerTest, FluidInitializersComputingIterationsAreOK)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<DiagnosticInitializer> diagInit = factory->createDiagnosticInitializer();

    auto expectedSize = 0;
    auto firstIter    = 20000000000;
    auto lastIter     = 0;

    for (auto i : std::array<std::size_t, 2>{{0, 1}})
    {
        auto size = diagInit->fluidInitializers[i].computingIterations.size();

        if (diagInit->fluidInitializers[i].diagName == "FluidDiagnostics1")
        {
            expectedSize = 201;
            firstIter    = 0;
            lastIter     = 1000;
        }
        else if (diagInit->fluidInitializers[i].diagName == "FluidDiagnostics2")
        {
            expectedSize = 201;
            firstIter    = 0;
            lastIter     = 1000;
        }

        ASSERT_EQ(expectedSize, size);
        ASSERT_EQ(firstIter, diagInit->fluidInitializers[i].computingIterations[0]);
        ASSERT_EQ(lastIter, diagInit->fluidInitializers[i].computingIterations[size - 1]);
    }
}




TEST(AsciiInitializerTest, FluidInitializersWrittingIterationsAreOK)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<DiagnosticInitializer> diagInit = factory->createDiagnosticInitializer();

    auto expectedSize = 0;
    auto firstIter    = 20000000000;
    auto lastIter     = 0;

    for (auto i : std::array<std::size_t, 2>{{0, 1}})
    {
        auto size = diagInit->fluidInitializers[i].writingIterations.size();

        if (diagInit->fluidInitializers[i].diagName == "FluidDiagnostics1")
        {
            expectedSize = 101;
            firstIter    = 0;
            lastIter     = 1000;
        }
        else if (diagInit->fluidInitializers[i].diagName == "FluidDiagnostics2")
        {
            expectedSize = 101;
            firstIter    = 0;
            lastIter     = 1000;
        }

        ASSERT_EQ(expectedSize, size);
        ASSERT_EQ(firstIter, diagInit->fluidInitializers[i].writingIterations[0]);
        ASSERT_EQ(lastIter, diagInit->fluidInitializers[i].writingIterations[size - 1]);
    }
}




TEST(AsciiInitializerTest, EMInitializersComputingIterationsAreOK)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<DiagnosticInitializer> diagInit = factory->createDiagnosticInitializer();

    auto expectedSize = 0;
    auto firstIter    = 20000000000;
    auto lastIter     = 0;

    for (auto i : std::array<std::size_t, 2>{{0, 1}})
    {
        auto size = diagInit->emInitializers[i].computingIterations.size();

        if (diagInit->emInitializers[i].diagName == "ElectromagDiagnostics1")
        {
            expectedSize = 201;
            firstIter    = 0;
            lastIter     = 1000;
        }
        else if (diagInit->emInitializers[i].diagName == "ElectromagDiagnostics2")
        {
            expectedSize = 101;
            firstIter    = 0;
            lastIter     = 1000;
        }

        ASSERT_EQ(expectedSize, size);
        ASSERT_EQ(firstIter, diagInit->emInitializers[i].computingIterations[0]);
        ASSERT_EQ(lastIter, diagInit->emInitializers[i].computingIterations[size - 1]);
    }
}




TEST(AsciiInitializerTest, EMInitializersWrittingIterationsAreOK)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<DiagnosticInitializer> diagInit = factory->createDiagnosticInitializer();

    auto expectedSize = 0;
    auto firstIter    = 20000000000;
    auto lastIter     = 0;

    for (auto i : std::array<std::size_t, 2>{{0, 1}})
    {
        auto size = diagInit->emInitializers[i].writingIterations.size();

        if (diagInit->emInitializers[i].diagName == "ElectromagDiagnostics1")
        {
            expectedSize = 101;
            firstIter    = 0;
            lastIter     = 1000;
        }
        else if (diagInit->emInitializers[i].diagName == "ElectromagDiagnostics2")
        {
            expectedSize = 51;
            firstIter    = 0;
            lastIter     = 1000;
        }

        ASSERT_EQ(expectedSize, size);
        ASSERT_EQ(firstIter, diagInit->emInitializers[i].writingIterations[0]);
        ASSERT_EQ(lastIter, diagInit->emInitializers[i].writingIterations[size - 1]);
    }
}



TEST(AsciiInitializerTest, EMInitializersTypesAreOK)
{
    std::unique_ptr<SimulationInitializerFactory> factory{new AsciiInitializerFactory{"phare.ini"}};
    std::unique_ptr<DiagnosticInitializer> diagInit = factory->createDiagnosticInitializer();

    for (auto i : std::array<std::size_t, 2>{{0, 1}})
    {
        if (diagInit->emInitializers[i].diagName == "ElectromagDiagnostics1")
        {
            ASSERT_EQ("E", diagInit->emInitializers[i].typeName);
        }
        else if (diagInit->emInitializers[i].diagName == "ElectromagDiagnostics2")
        {
            ASSERT_EQ("B", diagInit->emInitializers[i].typeName);
        }
    }
}




int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
