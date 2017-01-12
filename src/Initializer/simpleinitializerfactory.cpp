#include "simpleinitializerfactory.h"
#include "Initializer/fluidparticleinitializer.h"


SimpleInitializerFactory::SimpleInitializerFactory()
    : layout_{ {0.1,0.,0.}, {42, 0, 0}, 1, "yee", Point{0.,0.,0.}, 2}, // hard-coded... will come from input somehow
      dt_{0.1}
{

}



/* below are just stupid functions to make this initializer work
   these functions are intended to be passed to the fluid initializer
   we have to imagin that in reality these functions would come from other
   modules. For instance, the PythonIntializerFactory would read these functions
   from python scripts..
*/

double densityProton1(double x, double y, double z)
{
    return 1.;
}


double densityProton2(double x, double y, double z)
{
    return 0.25;
}


double thermalSpeedProton1(double x, double y, double z)
{
    return 0.25;
}


double thermalSpeedProton2(double x, double y, double z)
{
    return 0.25;
}


void bulkVelocityProton1(double x, double y, double z, std::array<double,3> vec)
{
    vec[0] = 0.;
    vec[0] = 0.;
    vec[0] = 0.;
}

void bulkVelocityProton2(double x, double y, double z, std::array<double,3> vec)
{
    vec[0] = 0.;
    vec[0] = 0.;
    vec[0] = 0.;
}


void magneticField(double x, double y, double z, std::array<double,3> vec)
{
    vec[0] = 0.;
    vec[0] = 0.;
    vec[0] = 0.;
}


void electricField(double x, double y, double z, std::array<double,3> vec)
{
    vec[0] = 0.;
    vec[0] = 0.;
    vec[0] = 0.;
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


std::unique_ptr<SolverInitializer> SimpleInitializerFactory::createSolverInitializer() const
{
    const uint32 nbrSpecies = 2;
    const std::string pusher = "modifiedBoris" ;

    const std::vector<uint32>  interpolationOrders = {2, 2} ;


    std::unique_ptr<SolverInitializer> solverInitPtr{ new SolverInitializer{} };

    solverInitPtr->nbrSpecies = nbrSpecies ;
    solverInitPtr->pusherType = pusher ;
    solverInitPtr->interpolationOrders = interpolationOrders ;


    return  solverInitPtr;

}


std::unique_ptr<ElectromagInitializer> SimpleInitializerFactory::createElectromagInitializer() const
{
    return nullptr;
}





std::unique_ptr<OhmInitializer> SimpleInitializerFactory::createOhmInitializer() const
{
    return nullptr;
}



GridLayout const& SimpleInitializerFactory::gridLayout() const
{
    return layout_;
}



double SimpleInitializerFactory::timeStep() const
{
    return dt_;
}


