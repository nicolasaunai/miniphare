
#include <iostream>
#include <random>

#include "fluidparticleinitializer.h"



FluidParticleInitializer::FluidParticleInitializer(GridLayout const& layout,
                                                   ScalarFunction densityProfile,
                                                   VectorFunction bulkVelocityProfile,
                                                   ScalarFunction thermalSpeedProfile,
                                                   uint32 nbrPartPerCell,
                                                   double particleCharge)
    : ParticleInitializer {},
      layout_{layout},
      density{densityProfile}, bulkVelocity{bulkVelocityProfile},
      thermalSpeed{thermalSpeedProfile},
      particleCharge_{particleCharge},
      nbrParticlePerCell_{nbrPartPerCell}
{

}





void maxwellianVelocity(std::array<double, 3> V, double Vth,
                        std::mt19937_64 generator,
                        std::array<double,3> partVelocity)
{
    std::normal_distribution<> maxwellX(V[0], Vth*Vth);
    std::normal_distribution<> maxwellY(V[1], Vth*Vth);
    std::normal_distribution<> maxwellZ(V[2], Vth*Vth);

    partVelocity[0] = maxwellX(generator);
    partVelocity[1] = maxwellY(generator);
    partVelocity[2] = maxwellZ(generator);
}





void FluidParticleInitializer::loadParticles1D_(std::vector<Particle>& particles) const
{
    std::cout << "FluidParticleInitializer : loading particles for 1D run... ";
    double dx;
    dx = layout_.dx();

    /* get indices start and stop. we take primal/primal/primal because
       that is what GridLayout::cellCenteredCoordinate() requires */
    uint32 ix0 = layout_.physicalStartIndex(QtyCentering::primal, Direction::X);
    uint32 ix1 = layout_.physicalEndIndex(QtyCentering::primal,   Direction::X);

    double cellVolume = dx;
    Point origin = layout_.origin();

    // random seed and generator needed to load maxwellian velocity
    // and random position with the cell
    std::random_device randSeed;
    std::mt19937_64 generator(randSeed());

    // beware: we're looping over the cell but use primal indices because of
    // GridLayout::cellCenteredCoordinates
    // therefore i(x,y,z)1 must be excluded.

    for (uint32 ix = ix0; ix < ix1; ++ix)
    {
        Point coord;                // cell physical coordinate
        double x;                   // cell coordinate along x
        double n;                   // cell centered density
        double Vth;                 // cell centered thermal speed
        std::array<double,3> V;     // cell centered bulk velocity
        double cellWeight;          // weight for all particles in this cell
        std::array<double,3> particleVelocity;

        // get the coordinate of the current cell
        coord  = layout_.cellCenteredCoordinates(ix,0,0);
        x      = coord.x_;

        // now get density, velocity and thermal speed values
        n      = density(x, origin.y_, origin.z_);
        V = bulkVelocity(x, origin.y_, origin.z_);
        Vth    = thermalSpeed(x, origin.y_, origin.z_);

        cellWeight = n*cellVolume / nbrParticlePerCell_;
        std::uniform_real_distribution<float> randPosX(0., dx);

        for (uint32 ipart=0; ipart < nbrParticlePerCell_; ++ipart)
        {
            maxwellianVelocity(V, Vth, generator, particleVelocity);

            std::array<float, 3> delta = { { randPosX(generator), 0. , 0. } };

            Particle tmpParticle(cellWeight,
                                 particleCharge_,
                                 {ix, 0, 0},
                                 delta,
                                 particleVelocity );

            particles.push_back(std::move(tmpParticle));
        }
    }
    std::cout << " OK!" << std::endl;
}






void FluidParticleInitializer::loadParticles2D_(std::vector<Particle>& particles) const
{
    std::cout << "2D particle loader\n";
    double dx, dy;
    dx = layout_.dx();
    dy = layout_.dy();

    /* get indices start and stop. we take primal/primal/primal because
       that is what GridLayout::cellCenteredCoordinate() requires */
    uint32 ix0 = layout_.physicalStartIndex(QtyCentering::primal, Direction::X);
    uint32 ix1 = layout_.physicalEndIndex(QtyCentering::primal,   Direction::X);
    uint32 iy0 = layout_.physicalStartIndex(QtyCentering::primal, Direction::Y);
    uint32 iy1 = layout_.physicalEndIndex(QtyCentering::primal,   Direction::Y);

    double cellVolume = dx*dy;
    Point origin = layout_.origin();

    // random seed and generator needed to load maxwellian velocity
    // and random position with the cell
    std::random_device randSeed;
    std::mt19937_64 generator(randSeed());

    // beware: we're looping over the cell but use primal indices because of
    // GridLayout::cellCenteredCoordinates
    // therefore i(x,y,z)1 must be excluded.

    for (uint32 ix = ix0; ix < ix1; ++ix)
    {
        for (uint32 iy = iy0; iy < iy1; ++iy)
        {
            Point coord;                // cell physical coordinate
            double x,y;                 // cell coordinate along x and y
            double n;                   // cell centered density
            double Vth;                 // cell centered thermal speed
            std::array<double,3> V;     // cell centered bulk velocity
            double cellWeight;          // weight for all particles in this cell
            std::array<double,3> particleVelocity;

            // get the coordinate of the current cell
            coord  = layout_.cellCenteredCoordinates(ix,iy,0);
            x      = coord.x_;
            y      = coord.y_;

            // now get density, velocity and thermal speed values
            n      = density(x, y, origin.z_);
            V = bulkVelocity(x, y, origin.z_);
            Vth    = thermalSpeed(x, y, origin.z_);

            cellWeight = n*cellVolume / nbrParticlePerCell_;
            std::uniform_real_distribution<float> randPosX(0., dx);
            std::uniform_real_distribution<float> randPosY(0., dy);

            for (uint32 ipart=0; ipart < nbrParticlePerCell_; ++ipart)
            {
                maxwellianVelocity(V, Vth, generator, particleVelocity);

                std::array<float, 3> delta =
                 { { randPosX(generator), randPosY(generator), 0. } };

                Particle tmpParticle(cellWeight,
                                     particleCharge_,
                                     {ix, iy, 0},
                                     delta,
                                     particleVelocity );

                particles.push_back(std::move(tmpParticle));
            }
        }
    }
}





void FluidParticleInitializer::loadParticles3D_(std::vector<Particle>& particles) const
{
    std::cout << "3D particle loader\n";
    double dx, dy, dz;
    dx = layout_.dx();
    dy = layout_.dy();
    dz = layout_.dz();

    /* get indices start and stop. we take primal/primal/primal because
       that is what GridLayout::cellCenteredCoordinate() requires */
    uint32 ix0 = layout_.physicalStartIndex(QtyCentering::primal, Direction::X);
    uint32 ix1 = layout_.physicalEndIndex(QtyCentering::primal,   Direction::X);
    uint32 iy0 = layout_.physicalStartIndex(QtyCentering::primal, Direction::Y);
    uint32 iy1 = layout_.physicalEndIndex(QtyCentering::primal,   Direction::Y);
    uint32 iz0 = layout_.physicalStartIndex(QtyCentering::primal, Direction::Z);
    uint32 iz1 = layout_.physicalEndIndex(QtyCentering::primal,   Direction::Z);

    double cellVolume = dx*dy*dz;
    Point origin = layout_.origin();

    // random seed and generator needed to load maxwellian velocity
    // and random position with the cell
    std::random_device randSeed;
    std::mt19937_64 generator(randSeed());

    // beware: we're looping over the cell but use primal indices because of
    // GridLayout::cellCenteredCoordinates
    // therefore i(x,y,z)1 must be excluded.

    for (uint32 ix = ix0; ix < ix1; ++ix)
    {
        for (uint32 iy = iy0; iy < iy1; ++iy)
        {
            for (uint32 iz = iz0; iz < iz1; ++iz)
            {
                Point coord;                // cell physical coordinate
                double x,y,z;               // cell coordinate along x and y
                double n;                   // cell centered density
                double Vth;                 // cell centered thermal speed
                std::array<double,3> V;     // cell centered bulk velocity
                double cellWeight;          // weight for all particles in this cell
                std::array<double,3> particleVelocity;

                // get the coordinate of the current cell
                coord  = layout_.cellCenteredCoordinates(ix,iy,0);
                x      = coord.x_;
                y      = coord.y_;
                z      = coord.z_;

                // now get density, velocity and thermal speed values
                n      = density(x, y, z);
                V = bulkVelocity(x, y, z);
                Vth    = thermalSpeed(x, y, z);
                cellWeight = n * cellVolume / nbrParticlePerCell_;

                std::uniform_real_distribution<float> randPosX(0., dx);
                std::uniform_real_distribution<float> randPosY(0., dy);
                std::uniform_real_distribution<float> randPosZ(0., dz);

                for (uint32 ipart=0; ipart < nbrParticlePerCell_; ++ipart)
                {
                    maxwellianVelocity(V, Vth, generator, particleVelocity);

                    std::array<float, 3> delta =
                     { { randPosX(generator), randPosY(generator), randPosZ(generator) } };

                    Particle tmpParticle(cellWeight,
                                         particleCharge_,
                                         {ix, iy, iz},
                                         delta,
                                          particleVelocity );

                    particles.push_back(std::move(tmpParticle));
                }
            }
        }
    }
}







 /**
  * @brief FluidParticleInitializer::loadParticles loads an array of Particle
  * following density, velocity and temperature profiles and a local
  * Maxwellian distirbution function
  * @param particles is the vector that will be filled with particles
  */
 void FluidParticleInitializer::loadParticles(std::vector<Particle>& particles) const
 {
    std::cout << "FluidParticleInitializer : I'm loading particles with Fluid profiles" << std::endl;
    std::cout << "total number of particles : " << particles.size() << std::endl;
    switch (layout_.nbDimensions())
    {
    case 1:
        loadParticles1D_(particles);
        break;

    case 2:
        loadParticles2D_(particles);
        break;

    case 3:
        loadParticles3D_(particles);
        break;
    }

    std::cout << "total number of particles : " << particles.size() << std::endl;
 }


 FluidParticleInitializer::~FluidParticleInitializer()
 {

 }





