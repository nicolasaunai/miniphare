
#include <memory>

#include "core/BoundaryConditions/domainboundarycondition.h"
#include "core/BoundaryConditions/periodicdomainboundary.h"

#include "fluidparticleinitializer.h"
#include "simpleinitializerfactory.h"

#include "amr/Splitting/splittingstrategyfactory.h"


#include <cmath>


static const uint32 interpOrderConstant = 2;
// static const uint32 refineFactorConstant = 2;


static const std::string defaultSplitMethod = "splitOrderN_RF2";

static const double pi = 3.14159;

static const double dx = 0.3;
static const int32 nx  = 2048;
static const double Lx = nx * dx;

static const double dt   = 1.e-3;
static const int32 nstep = 40; // 10000


// le mode de notre perturbation
static const double k3 = 2 * pi / (Lx / 4.);
static const double k4 = 2 * pi / (Lx / 2.);
static const double k5 = 2 * pi / Lx;


SimpleInitializerFactory::SimpleInitializerFactory()
    : timeManager_{0.01, 0., 10.}
    , layout_{{{dx, 0., 0.}}, {{nx, 0, 0}}, 1, "yee", Point{0., 0., 0.}, interpOrderConstant}
    ,
    // hard-coded... will come from input somehow
    interpolationOrder_{interpOrderConstant}
    , pusher_{"modifiedBoris"}
    , splitMethods_{std::vector<std::string>{2, defaultSplitMethod}}
{
}



/* below are just stupid functions to make this initializer work
   these functions are intended to be passed to the fluid initializer
   we have to imagin that in reality these functions would come from other
   modules. For instance, the PythonIntializerFactory would read these functions
   from python scripts..
*/

class Density : public ScalarFunction
{
public:
    virtual double operator()(double x, double y, double z) override
    {
        (void)x;
        (void)y;
        (void)z;
        double density = 1.;
        return density;
    }
};


class ThermalSpeed : public ScalarFunction
{
    virtual double operator()(double x, double y, double z) override
    {
        (void)x;
        (void)y;
        (void)z;
        return 0.001;
    }
};


class BulkVelocity : public VectorFunction
{
    virtual std::array<double, 3> operator()(double x, double y, double z) override
    {
        std::array<double, 3> vec;

        (void)x;
        (void)y;
        (void)z;

        vec[0] = 0.;
        vec[1] = 0.;
        vec[2] = 0.;
        return vec;
    }
};



class MagneticField : public VectorFunction
{
    virtual std::array<double, 3> operator()(double x, double y, double z) override
    {
        std::array<double, 3> vec;

        (void)x;
        (void)y;
        (void)z;

        vec[0] = 1.;
        vec[1] = 0.1 * (std::sin(k3 * x) + std::sin(k4 * x) + std::sin(k5 * x));
        vec[2] = 0.1 * (std::cos(k3 * x) + std::cos(k4 * x) + std::cos(k5 * x));
        return vec;
    }
};


class ElectricField : public VectorFunction
{
    virtual std::array<double, 3> operator()(double x, double y, double z) override
    {
        std::array<double, 3> vec;

        (void)x;
        (void)y;
        (void)z;

        vec[0] = 0.;
        vec[1] = 0.;
        vec[2] = 0.;
        return vec;
    }
};



/* -------------------------- end of hard coded functions --------------------- */




/**
 * @brief SimpleInitializerFactory::createIonsInitializer
 *
 * This function hard-codes the creation of an IonInitializer. Two proton
 * species are created with FluidParticleInitializer with hard-coded
 * profiles defined in this file.
 * @return
 */
std::unique_ptr<IonsInitializer> SimpleInitializerFactory::createIonsInitializer() const
{
    const uint32 nbrSpecies     = 1;   // 2
    const uint32 nbrPartPerCell = 100; // 100
    double chargeProton1        = 1.;
    // double chargeProton2        = 1.;

    // should be obtained from
    // the factory somehow...

    std::unique_ptr<IonsInitializer> ionInitPtr{new IonsInitializer{}};

    ionInitPtr->nbrSpecies = nbrSpecies;
    ionInitPtr->masses.push_back(1.);
    // ionInitPtr->masses.push_back(1.);
    ionInitPtr->names.push_back("proton1");
    // ionInitPtr->names.push_back("proton2");

    std::unique_ptr<ScalarFunction> density{new Density{}};
    std::unique_ptr<ScalarFunction> thermalSpeed{new ThermalSpeed{}};
    std::unique_ptr<VectorFunction> bulkVelocity{new BulkVelocity{}};

    // TODO those rvalues should be moved so ParticleInitializer should have noexcept move Ctor.
    ionInitPtr->particleInitializers.push_back(std::unique_ptr<ParticleInitializer>{
        new FluidParticleInitializer{layout_, std::move(density), std::move(bulkVelocity),
                                     std::move(thermalSpeed), nbrPartPerCell, chargeProton1}});

    //    ionInitPtr->particleInitializers.push_back(std::unique_ptr<ParticleInitializer>{
    //        new FluidParticleInitializer{layout_, densityProton2, bulkVelocityProton2,
    //                                     thermalSpeedProton2, nbrPartPerCell, chargeProton2}});

    return ionInitPtr;
}




/**
 * @brief SimpleInitializerFactory::createSolverInitializer creates a SolverInitializer
 * for simpled 1D simulation with 2 species with 2nd order interpolation and modified Boris pushers
 * @return
 */
std::unique_ptr<SolverInitializer> SimpleInitializerFactory::createSolverInitializer() const
{
    std::unique_ptr<SolverInitializer> solverInitPtr{new SolverInitializer{}};

    solverInitPtr->pusherType         = pusher_;
    solverInitPtr->interpolationOrder = interpolationOrder_;

    return solverInitPtr;
}



/**
 * @brief SimpleInitializerFactory::createBoundaryCondition creates a
 * BoundaryCondition that is a simple periodic domain boundary condition for 1D run.
 * @return
 */
std::unique_ptr<BoundaryCondition> SimpleInitializerFactory::createBoundaryCondition() const
{
    // return hard coded domain periodic boundary condition
    std::vector<DomainBoundaryCondition::BoundaryInfo> boundaries(2);

    // "first" is the edge coordinate
    boundaries[0].first = Edge::Xmin;
    boundaries[1].first = Edge::Xmax;

    // "second" is the type of boundary, here periodic
    boundaries[0].second = BoundaryType::Periodic;
    boundaries[1].second = BoundaryType::Periodic;

    std::unique_ptr<BoundaryCondition> bc{new DomainBoundaryCondition{layout_, boundaries}};

    return bc;
}




/**
 * @brief SimpleInitializerFactory::createElectromagInitializer return
 *  a ElectromagInitializer for 1D periodic simulation.
 * @return
 */
std::unique_ptr<ElectromagInitializer> SimpleInitializerFactory::createElectromagInitializer() const
{
    std::unique_ptr<VectorFunction> magneticField{new MagneticField{}};
    std::unique_ptr<VectorFunction> electricField{new ElectricField{}};

    std::unique_ptr<ElectromagInitializer> eminit{new ElectromagInitializer{
        layout_, std::move(electricField), std::move(magneticField), "_EField", "_BField"}};

    std::cout << "creating Simple ElectromagInitializer" << std::endl;

    return eminit;
}



/**
 * @brief SimpleInitializerFactory::createDiagnosticInitializer is a hard-coded
 * DiagnosticInitializer
 */
std::unique_ptr<DiagnosticInitializer> SimpleInitializerFactory::createDiagnosticInitializer() const
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



std::unique_ptr<Time> SimpleInitializerFactory::createTimeManager() const
{
    return std::unique_ptr<Time>{new Time{dt, 0., nstep * dt}};
}
