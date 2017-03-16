
#include <memory>

#include "simpleinitializerfactory.h"
#include "Initializer/fluidparticleinitializer.h"
#include "BoundaryConditions/domainboundarycondition.h"
#include "BoundaryConditions/periodicdomainboundary.h"


SimpleInitializerFactory::SimpleInitializerFactory()
    : layout_{ {{0.1,0.,0.}}, {{42, 0, 0}}, 1, "yee", Point{0.,0.,0.}, 2},
      // hard-coded... will come from input somehow
      dt_{0.1}
{

}



/* below are just stupid functions to make this initializer work
   these functions are intended to be passed to the fluid initializer
   we have to imagin that in reality these functions would come from other
   modules. For instance, the PythonIntializerFactory would read these functions
   from python scripts.. */
double densityProton1(double x, double y, double z)
{
    (void) x;
    (void) y;
    (void) z;

    return 1.;
}


double densityProton2(double x, double y, double z)
{
    (void) x;
    (void) y;
    (void) z;
    return 0.25;
}


double thermalSpeedProton1(double x, double y, double z)
{
    (void) x;
    (void) y;
    (void) z;
    return 0.25;
}


double thermalSpeedProton2(double x, double y, double z)
{
    (void) x;
    (void) y;
    (void) z;
    return 0.25;
}


std::array<double,3> bulkVelocityProton1(double x, double y, double z)
{
    std::array<double,3> vec;
    (void) x;
    (void) y;
    (void) z;
    vec[0] = 0.;
    vec[1] = 0.;
    vec[2] = 0.;
    return vec;
}

std::array<double,3> bulkVelocityProton2(double x, double y, double z)
{
    std::array<double,3> vec;
    (void) x;
    (void) y;
    (void) z;
    vec[0] = 0.;
    vec[1] = 0.;
    vec[2] = 0.;
    return vec;
}


std::array<double,3>  magneticField(double x, double y, double z)
{
    std::array<double,3> vec;
    (void) x;
    (void) y;
    (void) z;
    vec[0] = 1.;
    vec[1] = 0.;
    vec[2] = 0.;
    return vec;
}


std::array<double,3> electricField(double x, double y, double z)
{
    std::array<double,3> vec;
    (void) x;
    (void) y;
    (void) z;
    vec[0] = 0.;
    vec[1] = 0.;
    vec[2] = 0.;
    return vec;
}
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
    const uint32 nbrSpecies = 2;
    const uint32 nbrPartPerCell = 100;
    double chargeProton1 = 1., chargeProton2 = 1.;

    // should be obtained from
    // the factory somehow...

    std::unique_ptr<IonsInitializer> ionInitPtr{ new IonsInitializer{} };

    ionInitPtr->nbrSpecies = nbrSpecies;
    ionInitPtr->masses.push_back(1.);
    ionInitPtr->masses.push_back(1.);
    ionInitPtr->names.push_back("proton1");
    ionInitPtr->names.push_back("proton2");


    // TODO those rvalues should be moved so ParticleInitializer should have noexcept move Ctor.
    ionInitPtr->particleInitializers.push_back( std::unique_ptr<ParticleInitializer>
                                                    {new FluidParticleInitializer{layout_,
                                                                densityProton1,
                                                                bulkVelocityProton1,
                                                                thermalSpeedProton1,
                                                                nbrPartPerCell,
                                                                chargeProton1} } );

    ionInitPtr->particleInitializers.push_back( std::unique_ptr<ParticleInitializer>
                                                    {new FluidParticleInitializer{layout_,
                                                     densityProton2, bulkVelocityProton2,
                                                     thermalSpeedProton2,
                                                     nbrPartPerCell,
                                                     chargeProton2} } );

    return ionInitPtr;

}




/**
 * @brief SimpleInitializerFactory::createSolverInitializer creates a SolverInitializer for simpled 1D simulation with 2 species with 2nd order interpolation and modified Boris pushers
 * @return
 */
std::unique_ptr<SolverInitializer> SimpleInitializerFactory::createSolverInitializer() const
{
    const std::string pusher = "modifiedBoris" ;

    const std::vector<uint32>  interpolationOrders = {1, 1} ;

    std::unique_ptr<SolverInitializer> solverInitPtr{ new SolverInitializer{} };

    solverInitPtr->pusherType = pusher ;
    solverInitPtr->interpolationOrders = interpolationOrders ;

    return  solverInitPtr;
}



/**
 * @brief SimpleInitializerFactory::createBoundaryCondition creates a BoundaryCondition that is a simple periodic domain boundary condition for 1D run.
 * @return
 */
std::unique_ptr<BoundaryCondition>SimpleInitializerFactory::createBoundaryCondition() const
{
    // return hard coded domain periodic boundary condition
    std::vector<DomainBoundaryCondition::BoundaryInfo> boundaries(2);

    // "first" is the edge coordinate
    boundaries[0].first = Edge::Xmin;
    boundaries[1].first = Edge::Xmax;

    // "second" is the type of boundary, here periodic
    boundaries[0].second = BoundaryType::Periodic;
    boundaries[1].second = BoundaryType::Periodic;

    std::unique_ptr<BoundaryCondition> bc {new DomainBoundaryCondition{layout_, boundaries}};

    return bc;
}




/**
 * @brief SimpleInitializerFactory::createElectromagInitializer return a ElectromagInitializer for 1D periodic simulation. Bx=1, By=Bz=Ex=Ey=Ez=0.
 * @return
 */
std::unique_ptr<ElectromagInitializer> SimpleInitializerFactory::createElectromagInitializer() const
{

    std::unique_ptr<ElectromagInitializer> eminit {new ElectromagInitializer{layout_,
                    electricField,
                    magneticField, "_EMField", "_EMFields"} };

    std::cout << "creating Simple ElectromagInitializer" << std::endl;
    Point origin{0,0,0};

    for (uint32 iComponent=0; iComponent < 3; ++iComponent)
    {

        // ELECTRIC FIELD ----------------
        Field& Ei = eminit->E_.component(iComponent);
        uint32 iStart = layout_.ghostStartIndex(Ei, Direction::X);
        uint32 iEnd   = layout_.ghostEndIndex(  Ei, Direction::X);

        for (uint32 ix=iStart; ix <= iEnd; ++ix)
        {
            Point coord = layout_.fieldNodeCoordinates(Ei, origin, ix, 0, 0);
            std::array<double,3> E = electricField(coord.x_, origin.y_, origin.z_);
            Ei(ix) = E[iComponent];
        }

        // MEGNETIC FIELD ----------------
        Field& Bi = eminit->B_.component(iComponent);
        iStart = layout_.ghostStartIndex(Bi, Direction::X);
        iEnd   = layout_.ghostEndIndex(  Bi, Direction::X);

        for (uint32 ix=iStart; ix <= iEnd; ++ix)
        {
            Point coord = layout_.fieldNodeCoordinates(Bi, origin, ix, 0, 0);
            std::array<double,3> B = magneticField(coord.x_, origin.y_, origin.z_);
            Bi(ix) = B[iComponent];
        }
    }

    return eminit;
}





std::unique_ptr<OhmInitializer> SimpleInitializerFactory::createOhmInitializer() const
{
    return nullptr;
}



GridLayout const& SimpleInitializerFactory::gridLayout() const
{
    return layout_;
}


Box SimpleInitializerFactory::getBox() const
{
    return layout_.getBox() ;
}


double SimpleInitializerFactory::timeStep() const
{
    return dt_;
}


