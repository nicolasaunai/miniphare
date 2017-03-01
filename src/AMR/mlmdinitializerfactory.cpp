
#include "AMR/mlmdinitializerfactory.h"
#include "AMR/mlmdparticleinitializer.h"
#include "Electromag/electromag.h"
#include "Interpolator/interpolator.h"







/* below are just stupid functions to make this initializer work
   these functions are intended to be passed to the fluid initializer
   we have to imagin that in reality these functions would come from other
   modules. For instance, the PythonIntializerFactory would read these functions
   from python scripts..
*/

std::array<double,3>  zeroMagneticField(double x, double y, double z)
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


std::array<double,3> zeroElectricField(double x, double y, double z)
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





std::unique_ptr<IonsInitializer> MLMDInitializerFactory::createIonsInitializer() const
{
    /* this routine creates an ion initializer with a Patch Choice function. */
    std::unique_ptr<IonsInitializer> ionInitPtr{ new IonsInitializer{} };
    Ions const& parentIons = parentPatch_->ions();

    for (uint32 ispe=0; ispe < parentIons.nbrSpecies(); ++ispe)
    {
        Species const& species = parentIons.species(ispe);
        Box  parentCoordinates  = parentPatch_->coordinates();
        std::unique_ptr<ParticleSelector> selector{
            new isInBox{parentCoordinates, newPatchCoords_, layout_.dxdydz()} };

        std::unique_ptr<ParticleInitializer>
                particleInit{new MLMDParticleInitializer{species, std::move(selector) }};

        ionInitPtr->masses.push_back( parentIons.species(ispe).mass() );
        ionInitPtr->particleInitializers.push_back( std::move(particleInit) );
    }


    return ionInitPtr;
}




std::unique_ptr<ElectromagInitializer>
MLMDInitializerFactory::createElectromagInitializer() const
{

    Electromag const & parentElectromag = parentPatch_->data().EMfields() ;

    Interpolator interpolator(interpolationOrder_) ;

    // electricField and magneticField will not be used
    std::unique_ptr<ElectromagInitializer> eminit {
        new ElectromagInitializer{layout_,
                    zeroElectricField,
                    zeroMagneticField, "_EMField", "_EMFields"} };

    std::cout << "creating MLMD ElectromagInitializer" << std::endl;
//    Point origin{0,0,0};

    for (uint32 iComponent=0; iComponent < 3; ++iComponent)
    {

        // ELECTRIC FIELD ----------------
        Field& Ei = eminit->E_.component(iComponent);
        uint32 iStart = layout_.ghostStartIndex(Ei, Direction::X);
        uint32 iEnd   = layout_.ghostEndIndex(  Ei, Direction::X);

//        for (uint32 ix=iStart; ix <= iEnd; ++ix)
//        {
//            Point coord = layout_.fieldNodeCoordinates(Ei, origin, ix, 0, 0);
//            std::array<double,3> E = electricField(coord.x_, origin.y_, origin.z_);
//            Ei(ix) = E[iComponent];
//        }

        // MAGNETIC FIELD ----------------
        Field& Bi = eminit->B_.component(iComponent);
        iStart = layout_.ghostStartIndex(Bi, Direction::X);
        iEnd   = layout_.ghostEndIndex(  Bi, Direction::X);

//        for (uint32 ix=iStart; ix <= iEnd; ++ix)
//        {
//            Point coord = layout_.fieldNodeCoordinates(Bi, origin, ix, 0, 0);
//            std::array<double,3> B = magneticField(coord.x_, origin.y_, origin.z_);
//            Bi(ix) = B[iComponent];
//        }

    }

    return eminit;
}




std::unique_ptr<SolverInitializer> MLMDInitializerFactory::createSolverInitializer() const
{
    return nullptr;
}


std::unique_ptr<OhmInitializer> MLMDInitializerFactory::createOhmInitializer() const
{
    return nullptr;
}


std::unique_ptr<BoundaryCondition> MLMDInitializerFactory::createBoundaryCondition() const
{

    return nullptr;
}



GridLayout const& MLMDInitializerFactory::gridLayout() const
{
    return layout_;
}


Box MLMDInitializerFactory::getBox() const
{
    return layout_.getBox() ;
}


double MLMDInitializerFactory::timeStep() const
{
    return dt_;
}



