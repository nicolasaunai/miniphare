
#include <iostream>
#include <random>

#include "fluidparticleinitializer.h"
#include <utilities/print/outputs.h>
#include <utilities/utilities.h>


FluidParticleInitializer::FluidParticleInitializer(
    GridLayout const& layout, std::unique_ptr<ScalarFunction> densityProfile,
    std::unique_ptr<VectorFunction> bulkVelocityProfile,
    std::unique_ptr<VectorFunction> thermalSpeedProfile, uint32 nbrPartPerCell,
    double particleCharge, Basis base, std::unique_ptr<VectorFunction> magneticField)
    : ParticleInitializer{}
    , layout_{layout}
    , density_{std::move(densityProfile)}
    , bulkVelocity_{std::move(bulkVelocityProfile)}
    , thermalSpeed_{std::move(thermalSpeedProfile)}
    , magneticField_{std::move(magneticField)}
    , particleCharge_{particleCharge}
    , nbrParticlePerCell_{nbrPartPerCell}
    , base_{base}
{
}




void maxwellianVelocity(std::array<double, 3> V, std::array<double, 3> Vth,
                        std::mt19937_64 generator, std::array<double, 3>& partVelocity)
{
    std::normal_distribution<> maxwellX(V[0], Vth[0] * Vth[0]);
    std::normal_distribution<> maxwellY(V[1], Vth[1] * Vth[1]);
    std::normal_distribution<> maxwellZ(V[2], Vth[2] * Vth[2]);

    partVelocity[0] = maxwellX(generator);
    partVelocity[1] = maxwellY(generator);
    partVelocity[2] = maxwellZ(generator);
}




void FluidParticleInitializer::loadParticles1D_(std::vector<Particle>& particles) const
{
    Logger::Debug << "\t - 1D Fluid Particle Initializer\n";
    double dx;
    dx = layout_.dx();

    /* get indices start and stop. we take primal/primal/primal because
       that is what GridLayout::cellCenteredCoordinate() requires */
    uint32 ix0 = layout_.physicalStartIndex(QtyCentering::primal, Direction::X);
    uint32 ix1 = layout_.physicalEndIndex(QtyCentering::primal, Direction::X);

    double cellVolume = dx;
    Point origin      = layout_.origin();

    // random seed and generator needed to load maxwellian velocity
    // and random position with the cell
    std::random_device randSeed;
    // std::mt19937_64 generator(randSeed());
    std::mt19937_64 generator(1); // TODO constant seed should be usable for Debug mode.

    // beware: we're looping over the cell but use primal indices because of
    // GridLayout::cellCenteredCoordinates
    // therefore i(x,y,z)1 must be excluded.

    // grab references for convenience
    auto& density       = *density_;
    auto& bulkVelocity  = *bulkVelocity_;
    auto& thermalSpeed  = *thermalSpeed_;
    auto& magneticField = *magneticField_;

    for (uint32 ix = ix0; ix < ix1; ++ix)
    {
        Point coord;               // cell physical coordinate
        double x;                  // cell coordinate along x
        double n;                  // cell centered density
        std::array<double, 3> Vth; // cell centered thermal speed
        std::array<double, 3> V;   // cell centered bulk velocity
        double cellWeight;         // weight for all particles in this cell
        std::array<double, 3> particleVelocity;
        std::array<std::array<double, 3>, 3> basis;

        // get the coordinate of the current cell
        coord = layout_.cellCenteredCoordinates(ix, 0, 0);
        x     = coord.x;

        // now get density, velocity and thermal speed values
        n   = density(x, origin.y, origin.z);
        V   = bulkVelocity(x, origin.y, origin.z);
        Vth = thermalSpeed(x, origin.y, origin.z);

        cellWeight = n * cellVolume / nbrParticlePerCell_;
        std::uniform_real_distribution<float> randPosX(0., 1.);

        if (base_ == Basis::Magnetic)
        {
            auto B = magneticField(x, origin.y, origin.z);
            localMagneticBasis(B, basis);
        }

        for (uint32 ipart = 0; ipart < nbrParticlePerCell_; ++ipart)
        {
            maxwellianVelocity(V, Vth, generator, particleVelocity);

            if (base_ == Basis::Magnetic)
            {
                particleVelocity = basisTransform(basis, particleVelocity);
            }

            std::array<float, 3> delta = {{randPosX(generator), 0., 0.}};

            Particle tmpParticle(cellWeight, particleCharge_, {{static_cast<int32>(ix), 0, 0}},
                                 delta, particleVelocity);

            particles.push_back(std::move(tmpParticle));
        }
    }
}




void FluidParticleInitializer::loadParticles2D_(std::vector<Particle>& particles) const
{
    Logger::Debug << "\t - 2D Fluid Particle Initializer\n";
    double dx, dy;
    dx = layout_.dx();
    dy = layout_.dy();

    /* get indices start and stop. we take primal/primal/primal because
       that is what GridLayout::cellCenteredCoordinate() requires */
    uint32 ix0 = layout_.physicalStartIndex(QtyCentering::primal, Direction::X);
    uint32 ix1 = layout_.physicalEndIndex(QtyCentering::primal, Direction::X);
    uint32 iy0 = layout_.physicalStartIndex(QtyCentering::primal, Direction::Y);
    uint32 iy1 = layout_.physicalEndIndex(QtyCentering::primal, Direction::Y);

    double cellVolume = dx * dy;
    Point origin      = layout_.origin();

    // random seed and generator needed to load maxwellian velocity
    // and random position with the cell
    std::random_device randSeed;
    std::mt19937_64 generator(randSeed());

    // beware: we're looping over the cell but use primal indices because of
    // GridLayout::cellCenteredCoordinates
    // therefore i(x,y,z)1 must be excluded.

    // grab references for convenience
    auto& density       = *density_;
    auto& bulkVelocity  = *bulkVelocity_;
    auto& thermalSpeed  = *thermalSpeed_;
    auto& magneticField = *magneticField_;


    for (uint32 ix = ix0; ix < ix1; ++ix)
    {
        for (uint32 iy = iy0; iy < iy1; ++iy)
        {
            Point coord;               // cell physical coordinate
            double x, y;               // cell coordinate along x and y
            double n;                  // cell centered density
            std::array<double, 3> Vth; // cell centered thermal speed
            std::array<double, 3> V;   // cell centered bulk velocity
            double cellWeight;         // weight for all particles in this cell
            std::array<double, 3> particleVelocity;
            std::array<std::array<double, 3>, 3> basis;

            // get the coordinate of the current cell
            coord = layout_.cellCenteredCoordinates(ix, iy, 0);
            x     = coord.x;
            y     = coord.y;

            // now get density, velocity and thermal speed values
            n   = density(x, y, origin.z);
            V   = bulkVelocity(x, y, origin.z);
            Vth = thermalSpeed(x, y, origin.z);

            cellWeight = n * cellVolume / nbrParticlePerCell_;
            std::uniform_real_distribution<float> randPosX(0., 1.);
            std::uniform_real_distribution<float> randPosY(0., 1.);

            if (base_ == Basis::Magnetic)
            {
                auto B = magneticField(x, y, origin.z);
                localMagneticBasis(B, basis);
            }


            for (uint32 ipart = 0; ipart < nbrParticlePerCell_; ++ipart)
            {
                maxwellianVelocity(V, Vth, generator, particleVelocity);

                if (base_ == Basis::Magnetic)
                {
                    particleVelocity = basisTransform(basis, particleVelocity);
                }

                std::array<float, 3> delta = {{randPosX(generator), randPosY(generator), 0.}};

                Particle tmpParticle(cellWeight, particleCharge_,
                                     {{static_cast<int32>(ix), static_cast<int32>(iy), 0}}, delta,
                                     particleVelocity);

                particles.push_back(std::move(tmpParticle));
            }
        }
    }
}




void FluidParticleInitializer::loadParticles3D_(std::vector<Particle>& particles) const
{
    Logger::Debug << "\t - 3D Fluid Particle Initializer\n";
    double dx, dy, dz;
    dx = layout_.dx();
    dy = layout_.dy();
    dz = layout_.dz();

    /* get indices start and stop. we take primal/primal/primal because
       that is what GridLayout::cellCenteredCoordinate() requires */
    uint32 ix0 = layout_.physicalStartIndex(QtyCentering::primal, Direction::X);
    uint32 ix1 = layout_.physicalEndIndex(QtyCentering::primal, Direction::X);
    uint32 iy0 = layout_.physicalStartIndex(QtyCentering::primal, Direction::Y);
    uint32 iy1 = layout_.physicalEndIndex(QtyCentering::primal, Direction::Y);
    uint32 iz0 = layout_.physicalStartIndex(QtyCentering::primal, Direction::Z);
    uint32 iz1 = layout_.physicalEndIndex(QtyCentering::primal, Direction::Z);

    double cellVolume = dx * dy * dz;

    // random seed and generator needed to load maxwellian velocity
    // and random position with the cell
    std::random_device randSeed;
    std::mt19937_64 generator(randSeed());

    // beware: we're looping over the cell but use primal indices because of
    // GridLayout::cellCenteredCoordinates
    // therefore i(x,y,z)1 must be excluded.

    // grab references for convenience
    auto& density       = *density_;
    auto& bulkVelocity  = *bulkVelocity_;
    auto& thermalSpeed  = *thermalSpeed_;
    auto& magneticField = *magneticField_;

    for (uint32 ix = ix0; ix < ix1; ++ix)
    {
        for (uint32 iy = iy0; iy < iy1; ++iy)
        {
            for (uint32 iz = iz0; iz < iz1; ++iz)
            {
                Point coord;               // cell physical coordinate
                double x, y, z;            // cell coordinate along x and y
                double n;                  // cell centered density
                std::array<double, 3> Vth; // cell centered thermal speed
                std::array<double, 3> V;   // cell centered bulk velocity
                double cellWeight;         // weight for all particles in this cell
                std::array<double, 3> particleVelocity;
                std::array<std::array<double, 3>, 3> basis;

                // get the coordinate of the current cell
                coord = layout_.cellCenteredCoordinates(ix, iy, iz);
                x     = coord.x;
                y     = coord.y;
                z     = coord.z;

                // now get density, velocity and thermal speed values
                n          = density(x, y, z);
                V          = bulkVelocity(x, y, z);
                Vth        = thermalSpeed(x, y, z);
                cellWeight = n * cellVolume / nbrParticlePerCell_;

                std::uniform_real_distribution<float> randPosX(0., 1.);
                std::uniform_real_distribution<float> randPosY(0., 1.);
                std::uniform_real_distribution<float> randPosZ(0., 1.);

                if (base_ == Basis::Magnetic)
                {
                    auto B = magneticField(x, y, z);
                    localMagneticBasis(B, basis);
                }

                for (uint32 ipart = 0; ipart < nbrParticlePerCell_; ++ipart)
                {
                    maxwellianVelocity(V, Vth, generator, particleVelocity);

                    if (base_ == Basis::Magnetic)
                    {
                        particleVelocity = basisTransform(basis, particleVelocity);
                    }

                    std::array<float, 3> delta
                        = {{randPosX(generator), randPosY(generator), randPosZ(generator)}};

                    Particle tmpParticle(
                        cellWeight, particleCharge_,
                        {{static_cast<int32>(ix), static_cast<int32>(iy), static_cast<int32>(iz)}},
                        delta, particleVelocity);

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
    switch (layout_.nbDimensions())
    {
        case 1: loadParticles1D_(particles); break;

        case 2: loadParticles2D_(particles); break;

        case 3: loadParticles3D_(particles); break;
    }
    Logger::Debug << "\t - Number of particles loaded : " << particles.size() << "\n";
}


FluidParticleInitializer::~FluidParticleInitializer()
{
}
