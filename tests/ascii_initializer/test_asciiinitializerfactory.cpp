
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




int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
